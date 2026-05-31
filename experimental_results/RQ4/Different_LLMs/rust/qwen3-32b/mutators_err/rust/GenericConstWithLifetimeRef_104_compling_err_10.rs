use syn::parse_quote;
use syn::Item;
use syn::GenericParam;
use syn::Stmt;
use syn::Expr;
use syn::Type;
use syn::PathArguments;
use syn::GenericArgument;
use crate::mutator::Mutator;

pub struct GenericConstWithLifetimeRef_104;

impl Mutator for GenericConstWithLifetimeRef_104 {
    fn name(&self) -> &str {
        "GenericConstWithLifetimeRef_104"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(caller) = item {
                let has_lifetimes = caller.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_)));
                if !has_lifetimes {
                    continue;
                }
                
                let lifetime_name = caller.sig.generics.params.iter()
                    .find(|param| matches!(param, GenericParam::Lifetime(_)))
                    .and_then(|param| {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            Some(lifetime_param.lifetime.ident.clone())
                        } else {
                            None
                        }
                    });
                
                if lifetime_name.is_none() {
                    continue;
                }
                let lifetime_name = lifetime_name.unwrap();
                
                for stmt in &mut caller.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call_expr) = &mut *expr {
                            if let Expr::Path(path_expr) = call_expr.func.as_mut() {
                                let function_name = path_expr.path.segments.first().unwrap().ident.to_string();
                                
                                let function_opt = file.items.iter_mut().find(|item| {
                                    if let Item::Fn(func) = item {
                                        func.sig.ident == function_name
                                    } else {
                                        false
                                    }
                                });
                                
                                if let Some(Item::Fn(function)) = function_opt {
                                    let has_const_usize = function.sig.generics.params.iter().any(|param| {
                                        if let GenericParam::Const(const_param) = param {
                                            if let Type::Path(type_path) = const_param.ty.as_ref() {
                                                if let Some(segment) = type_path.path.segments.first() {
                                                    segment.ident == "usize"
                                                } else {
                                                    false
                                                }
                                            } else {
                                                false
                                            }
                                        } else {
                                            false
                                        }
                                    });
                                    
                                    if has_const_usize {
                                        if let PathArguments::AngleBracketed(angle_args) = &mut path_expr.path.segments.first().unwrap().arguments {
                                            for arg in &mut angle_args.args {
                                                if let GenericArgument::Const(const_arg) = arg {
                                                    if let Expr::Lit(lit_expr) = &mut *const_arg {
                                                        let replacement = parse_quote! {
                                                            {
                                                                let _: &'lifetime_name ();
                                                                #lit_expr
                                                            }
                                                        };
                                                        *const_arg = replacement;
                                                    }
                                                }
                                            }
                                        }
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
        ""
    }
}