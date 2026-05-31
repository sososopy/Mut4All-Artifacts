use syn::{Item, Meta, Expr, Stmt, Lit, LitInt, ExprLit};
use proc_macro2::{TokenTree, Literal, Span};
use crate::mutator::Mutator;

struct Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390;

impl Mutator for Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390 {
    fn name(&self) -> &str {
        "Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for attr in &mut struct_item.attrs {
                    if let Meta::List(list) = &attr.meta {
                        if list.path.is_ident("const") {
                            let mut tokens = list.tokens.clone().into_iter().collect::<Vec<_>>();
                            for (i, token) in tokens.iter().enumerate() {
                                if let TokenTree::Ident(ident) = token {
                                    if ident.to_string() == "generic" {
                                        // Replace generic type with non-generic type
                                        let non_generic_type = Literal::i64_suffixed(42);
                                        tokens[i] = TokenTree::Literal(non_generic_type);
                                    }
                                }
                            }
                            list.tokens = tokens.into_iter().collect();
                        }
                    }
                }
            } else if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Path(path) = expr {
                            if path.path.is_ident("generic") {
                                // Replace generic type with non-generic type
                                let non_generic_type = Expr::Lit(ExprLit {
                                    attrs: vec![],
                                    lit: Lit::Int(LitInt::new("42", Span::call_site())),
                                });
                                *expr = non_generic_type;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions that use generic types and replaces them with non-generic constant expressions. This transformation aims to test the compiler's ability to handle generic constant expressions and ensure that it can correctly replace them with non-generic constant expressions."
    }
}