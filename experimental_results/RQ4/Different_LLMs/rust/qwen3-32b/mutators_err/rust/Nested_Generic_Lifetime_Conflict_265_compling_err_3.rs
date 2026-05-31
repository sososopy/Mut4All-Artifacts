pub struct Nested_Generic_Lifetime_Conflict_265;

impl Mutator for Nested_Generic_Lifetime_Conflict_265 {
    fn name(&self) -> &str {
        "Nested_Generic_Lifetime_Conflict_265"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Item(item) = stmt {
                                if let syn::Item::Fn(nested_fn) = &mut *item {
                                    let new_lifetime = parse_quote! {'a};
                                    nested_fn.sig.generics.params.push(GenericParam::Lifetime(new_lifetime));
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