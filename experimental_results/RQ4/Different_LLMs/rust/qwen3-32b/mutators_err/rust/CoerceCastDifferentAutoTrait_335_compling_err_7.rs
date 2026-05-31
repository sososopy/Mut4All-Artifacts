pub struct CoerceCastDifferentAutoTrait_335;

impl Mutator for CoerceCastDifferentAutoTrait_335 {
    fn name(&self) -> &str {
        "CoerceCastDifferentAutoTrait_335"
    }
    fn mutate(&self, file: &mut syn::File) {
        CoerceCastVisitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct CoerceCastVisitor;

impl VisitMut for CoerceCastVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Cast(expr_cast) = expr {
            if let Type::Path(type_path) = &*expr_cast.ty {
                if let Some(segment) = type_path.path.segments.first() {
                    if segment.ident == "dyn" {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            let mut bounds = Punctuated::new();
                            for arg in &args.args {
                                if let GenericArgument::Type(ty) = arg {
                                    if let Type::Path(ty_path) = &*ty {
                                        let path = ty_path.path.clone();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            path,
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                        }));
                                    }
                                }
                            }
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                path: SynPath::from(Ident::new("Sync", Span::call_site())),
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                            }));
                            let new_ty = Type::TraitObject(syn::TypeTraitObject {
                                dyn_token: Some(token::Dyn::default()),
                                bounds,
                            });
                            *expr_cast.ty = Box::new(new_ty);
                        }
                    }
                }
            }
        }
        self.visit_expr_mut(expr);
    }
}