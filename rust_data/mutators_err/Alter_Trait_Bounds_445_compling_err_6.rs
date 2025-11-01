use syn::parse_quote;
use syn::visit_mut::VisitMut;
use syn::{File, ItemImpl, ImplItem, ItemFn, GenericParam, TypeParamBound};

pub struct Alter_Trait_Bounds_445;

impl crate::mutator::Mutator for Alter_Trait_Bounds_445 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_445"
    }
    fn mutate(&self, file: &mut File) {
        struct TraitBoundVisitor {
            extra_trait_added: bool,
        }

        impl VisitMut for TraitBoundVisitor {
            fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
                if !self.extra_trait_added {
                    let extra_trait: ImplItem = parse_quote! {
                        impl Extra for () {}
                    };
                    i.items.insert(0, extra_trait);
                    self.extra_trait_added = true;
                }
                syn::visit_mut::visit_item_impl_mut(self, i);
            }

            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                for param in &mut i.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let extra_bound: TypeParamBound = parse_quote!(Extra);
                        type_param.bounds.push(extra_bound);
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
        }

        let mut visitor = TraitBoundVisitor { extra_trait_added: false };
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with generic parameters having trait bounds and adds an additional trait, `Extra`, to these bounds. This tests the compiler's handling of additional and potentially conflicting trait requirements."
    }
}