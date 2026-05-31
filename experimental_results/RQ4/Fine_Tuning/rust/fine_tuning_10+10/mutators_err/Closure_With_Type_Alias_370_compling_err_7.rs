use crate::mutator::Mutator;
use syn::{parse_quote, Item, Type, ReturnType, TypePath, Stmt, Expr, token::Semi};

pub struct Closure_With_Type_Alias_370;

impl Mutator for Closure_With_Type_Alias_370 {
    fn name(&self) -> &str {
        "Closure_With_Type_Alias_370"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_alias_ident = None;
        for item in &file.items {
            if let Item::Type(type_alias) = item {
                if let Type::ImplTrait(_) = *type_alias.ty {
                    type_alias_ident = Some(type_alias.ident.clone());
                    break;
                }
            }
        }

        if let Some(alias_ident) = type_alias_ident {
            for item in &mut file.items {
                if let Item::Fn(func) = item {
                    if let ReturnType::Type(_, ret_type) = &func.sig.output {
                        if let Type::Path(TypePath { path, .. }) = &**ret_type {
                            if path.is_ident(&alias_ident) {
                                if let Some(stmt) = func.block.stmts.first_mut() {
                                    if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                                        let captured_var_decl: Stmt = parse_quote! {
                                            let captured_var = 10;
                                        };
                                        let new_closure_body: Expr = parse_quote! {
                                            || { let _ = captured_var; 0 }
                                        };
                                        func.block.stmts.insert(0, captured_var_decl);
                                        *stmt = Stmt::Expr(new_closure_body, None);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets type aliases using `impl Trait` and modifies closures within functions returning these aliases. It introduces variable capture within closures, enhancing complexity and testing type inference in Rust."
    }
}