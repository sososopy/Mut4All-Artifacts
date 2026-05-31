pub struct Modify_Self_Bounds_Add_AsyncBlock_Convert_Mutable_304;

impl Mutator for Modify_Self_Bounds_Add_AsyncBlock_Convert_Mutable_304 {
    fn name(&self) -> &str {
        "Modify_Self_Bounds_Add_AsyncBlock_Convert_Mutable_304"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if let Some(async_token) = &method.sig.asyncness {
                            if let Some(first_arg) = method.sig.inputs.first_mut() {
                                if let FnArg::Typed(pat_type) = first_arg {
                                    if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                        if let Type::ImplTrait(impl_trait) = &mut *type_ref.elem {
                                            let mut new_bounds = Punctuated::new();
                                            for bound in &impl_trait.bounds {
                                                if let TypeParamBound::Trait(trait_bound) = bound {
                                                    if trait_bound.path.is_ident("Deref") {
                                                        new_bounds.push(bound.clone());
                                                    }
                                                }
                                            }
                                            new_bounds.push(parse_quote!(Sync));
                                            new_bounds.push(parse_quote!(?Sized));
                                            impl_trait.bounds = new_bounds;
                                        }
                                    }
                                }
                            }

                            if let Some(first_arg) = method.sig.inputs.first_mut() {
                                if let FnArg::Typed(pat_type) = first_arg {
                                    if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                        type_ref.mutability = Some(token::Mut::default());
                                    }
                                }
                            }

                            if let Some(block) = &mut method.default {
                                block.stmts.insert(0, Stmt::Local(Local {
                                    attrs: Vec::new(),
                                    let_token: Default::default(),
                                    pat: Pat::Ident(PatIdent {
                                        ident: Ident::new("future", Span::call_site()),
                                        subpat: None,
                                        mutability: None,
                                        by_ref: None,
                                        attrs: Vec::new(),
                                    }),
                                    init: Some(LocalInit {
                                        eq_token: token::Eq::default(),
                                        expr: Box::new(Expr::Async(ExprAsync {
                                            attrs: Vec::new(),
                                            async_token: token::Async::default(),
                                            capture: None,
                                            block: parse_quote!({
                                                let _x = self.deref().clone();
                                                future.await
                                            }),
                                        })),
                                        diverge: None,
                                    }),
                                    semi_token: token::Semi::default(),
                                }));

                                let mut visitor = SelfFinder::default();
                                visitor.visit_block_mut(block);
                                for expr in visitor.found_exprs {
                                    *expr = parse_quote! {
                                        async move {
                                            let _x = self.deref().clone();
                                            future.await
                                        }
                                    };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator combines three transformations: (1) modifies the self parameter bounds in async trait methods by replacing existing trait bounds with a new set that includes Deref, (2) inserts an async block with self type projection and a future variable in the method body, and (3) converts the self reference to a mutable reference. This creates complex interactions between trait bounds, async execution, and mutability, aiming to maximize code coverage and test the compiler's handling of these features under mutation."
    }
}

struct SelfFinder {
    found_exprs: Vec<Expr>,
}

impl Default for SelfFinder {
    fn default() -> Self {
        Self { found_exprs: Vec::new() }
    }
}

impl VisitMut for SelfFinder {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        visit_mut::visit_expr_mut(self, expr);
        if let Expr::Path(expr_path) = expr {
            if expr_path.path.is_ident("self") {
                self.found_exprs.push(expr.clone());
            }
        }
    }
}