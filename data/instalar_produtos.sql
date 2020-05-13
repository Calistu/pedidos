insert into unidades (nome, sigla, multiplo)
values ('Folhas','Folhas',1),  
('Fardo','Fardo',1),
('Pacote','Pct',1),
('Unidade','UND',1),
('Caixa','CX',1);

insert into grupos(nome,pai) values 
('RAIZ',1), 
('PERCALUX',1), 
('CABECEADO',1), 
('COLA',1), 
('FITILHO ACETINADO',1), 
('LINHA',1), 
('PELICULA',1), 
('RESISTÊNCIA',1),
('ESPIRAIS', 1),
('PLASTIFICAÇÕES', 1),
('WERE-O', 1),
('CAPAS DE PP', 1),

('LISO', 2), 
('BICOLOR', 2), 
('METALIZADO', 2), 
('OURO/BRONZE', 2);

insert into terceiros (razao,doc,tipo_doc,ie,tipoc,tipo,cep,endereco,cidade,uf,numrua,tiporua, 
telefone,contatot ,celular , contatoc ,email,contatoe, transp_nome, transp_num,transp_logradouro ,
transp_cidade , transp_estado ,  transp_cep,  transp_telefone, transp_cnpj, transp_ie, transp_obs, obs,
frete_pago, vlr_frete_pago, prazo) 
values ('CLIENTE PARA TESTE', '512.146.788-58',  2, '123456789' , 'Cliente',  1,  '13067450', 'Rua Doutor Marcos Antonio Samartine', 
'Campinas', 'SP', 18, 1, '(19) 32818753', 'cliente', '(19) 987817853','calisto', 'email@email.com', 'contato', 'TRANSPORTADORA', 
18 ,'Praça da Sé, 108','São Paulo' ,'SP' , '01001900' ,'(null)' , '1234567890', '1234567890',  '(null)', '', 1, 200.00 ,'20'), 

('FORNECEDOR PARA TESTE', '123.146.744-58',  2, '123456789' , 'Fornecedor',  2,  '13067450', 'Rua Doutor Marcos Antonio Samartine', 
'Campinas', 'SP', 18, 1, '(19) 32818753', 'fornecedor', '(19) 987817853','calisto', 'email@email.com', 'contato', 'TRANSPORTADORA', 
18 ,'Praça da Sé, 108','São Paulo' ,'SP' , '01001900' ,'(null)' , '1234567890', '1234567890',  '(null)', '', 1, 200.00 ,'20');

insert into produtos(nome, preco_faturado, preco_vista, peso , unidade, grupo, observacoes) 
values('PERCALUX PAPEL PLASTIFICADO LISO - 1,30 LARG./25M - DIV. CORES', 225.00, 371.50, 0.0, 4, 1, ''),
('PERCALUX PAPEL PLASTIFICADO LISO - 1,30 LARG./25M - DIV. CORES', 225.00, 371.50, 0.0, 4, 1, ''),
('PERCALUX PAPEL PLASTIFICADO LISO - 1,30 LARG./50M - DIV. CORES', 371.50, 351.50, 0.0, 4, 1, ''),
('PERCALUX PAPEL PLASTIFICADO BICOLOR - 1,30 LARG./25M - CORES', 289.00, 289.00, 0.0, 4, 1, ''),
('PERCALUX PAPEL PLASTIFICADO METALIZADO - 1,30 LARG./25M - CORES', 251.00, 241.00, 0.0, 4, 1, ''),
('PERCALUX PAPEL PLASTIFICADO OURO/BRONZE - 1,30 LARG./25M', 225.00, 371.50, 0.0, 4, 1, ''),
('CABECEADO - FITA ALGODAO - ROLO 50M - DIVERSAS CORES', 225.00, 371.50, 0.0, 4, 1, '');