pub struct Replace_Const_Trait_Impl;

impl Mutator for Replace_Const_Trait_Impl {
    fn name(&self) -> &str {
        "Replace_Const_Trait_Impl"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(unsafety) = &mut impl_item.unsafety {
                    *unsafety = syn::token::Unsafe { span: Span::call_site() };
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const trait implementations with non-const trait implementations. This transformation tests the coherence of const trait implementations and the effects of replacing them with non-const implementations."
    }
}