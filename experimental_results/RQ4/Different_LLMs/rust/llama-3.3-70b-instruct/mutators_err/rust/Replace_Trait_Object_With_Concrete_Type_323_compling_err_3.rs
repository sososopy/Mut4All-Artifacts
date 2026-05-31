impl Mutator for Replace_Trait_Object_With_Concrete_Type_323 {
    fn mutate(&self, file_ast: &mut File) {
        let mut rng = thread_rng();
        for item in &mut file_ast.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Call(call) = &mut **expr {
                            if let Expr::Path(path) = &mut *call.expr {
                                if path.path.segments.len() == 1 {
                                    let segment = &path.path.segments[0];
                                    if segment.ident == "Box" {
                                        let ty = &call.args;
                                        if let Expr::Type(expr_ty) = ty {
                                            if let Type::TraitObject(trait_object) = &**expr_ty {
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
                                                            *expr = parse_quote! {
                                                                #concrete_type
                                                            };
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