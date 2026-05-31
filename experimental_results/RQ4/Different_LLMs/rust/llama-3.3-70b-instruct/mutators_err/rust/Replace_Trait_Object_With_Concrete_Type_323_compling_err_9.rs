use quote::quote;
use syn::{parse_quote, Item, Stmt, Expr, Type, TypeParamBound, File, Attribute};
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Trait_Object_With_Concrete_Type_323;

impl Mutator for Replace_Trait_Object_With_Concrete_Type_323 {
    fn name(&self) -> &str {
        "Replace_Trait_Object_With_Concrete_Type_323"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace trait object with concrete type"
    }

    fn mutate(&self, file_ast: &mut File) {
        let mut rng = thread_rng();
        for item in &mut file_ast.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                if path.path.segments.len() == 1 {
                                    let segment = &path.path.segments[0];
                                    if segment.ident == "Box" {
                                        let ty = &call.args;
                                        if let Some(expr_ty) = ty.get(0) {
                                            if let Expr::Type(expr_ty) = expr_ty {
                                                if let Type::TraitObject(trait_object) = &expr_ty.ty {
                                                    let bounds = &trait_object.bounds;
                                                    if bounds.len() == 1 {
                                                        let bound = &bounds[0];
                                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                                            let trait_path = &trait_bound.path;
                                                            if trait_path.segments.len() == 1 {
                                                                let trait_segment = &trait_path.segments[0];
                                                                let trait_ident = &trait_segment.ident;
                                                                let concrete_type = quote! {
                                                                    #trait_ident
                                                                };
                                                                *stmt = Stmt::Expr(parse_quote! {
                                                                    #concrete_type
                                                                }, None);
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
        }
    }
}