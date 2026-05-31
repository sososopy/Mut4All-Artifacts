use crate::mutator::Mutator;

pub struct Change_Impl_Trait_334;

impl Mutator for Change_Impl_Trait_334 {
    fn name(&self) -> &str {
        "Change_Impl_Trait_334"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &mut local.init {
                            if let Pat::Type(PatType { ty, .. }) = &*local.pat {
                                if let Type::Reference(type_reference) = ty.as_ref() {
                                    if let Type::ImplTrait(type_impl_trait) = type_reference.elem.as_ref() {
                                        let new_type = Type::ImplTrait(type_impl_trait.clone());
                                        local.pat = Box::new(Pat::Type(PatType {
                                            attrs: vec![],
                                            pat: local.pat.clone(),
                                            colon_token: Default::default(),
                                            ty: Box::new(new_type),
                                        }));
                                        if let Expr::Reference(expr_ref) = init.1.as_mut() {
                                            *init.1 = expr_ref.expr.clone();
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
        "The mutation operator targets local variable bindings using `impl Trait` in function bodies. It changes the type from `&impl Trait` to `impl Trait`, ensuring the assigned value is compatible with the new type."
    }
}