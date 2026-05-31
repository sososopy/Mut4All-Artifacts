use std::collections::HashSet;

pub struct Recursive_Trait_Method_Call_229;

impl Mutator for Recursive_Trait_Method_Call_229 {
    fn name(&self) -> &str {
        "Recursive_Trait_Method_Call_229"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_impls = vec![];
        let mut async_trait_methods = HashSet::new();

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        if method.sig.asyncness.is_some() {
                            async_trait_methods.insert(method.sig.ident.clone());
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for item in &item_impl.items {
                    if let ImplItem::Fn(method) = item {
                        if method.sig.asyncness.is_some() && !method.sig.inputs.is_empty() {
                            trait_impls.push((item_impl.self_ty.clone(), method.sig.ident.clone()));
                        }
                    }
                }
            }
        }

        for (self_ty, method_name) in trait_impls {
            if async_trait_methods.contains(&method_name) {
                let new_method_call: Expr = parse_quote! {
                    self.another_async_fn().await
                };

                for item in &mut file.items {
                    if let Item::Impl(item_impl) = item {
                        if item_impl.self_ty == self_ty {
                            for item in &mut item_impl.items {
                                if let ImplItem::Fn(method) = item {
                                    if method.sig.ident == method_name {
                                        method.block.stmts.push(Stmt::Expr(new_method_call.clone()));
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
        "The mutation operator identifies async methods within traits and their implementations. It introduces an indirect recursive call by adding a call to another async method within the same implementation, ensuring a cycle through .await points."
    }
}