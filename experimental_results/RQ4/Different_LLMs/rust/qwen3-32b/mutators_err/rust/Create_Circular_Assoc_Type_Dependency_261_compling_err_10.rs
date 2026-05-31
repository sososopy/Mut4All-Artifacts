use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Item;
use syn::ImplItem;
use rand::thread_rng;

pub struct Create_Circular_Assoc_Type_Dependency_261;

impl Mutator for Create_Circular_Assoc_Type_Dependency_261 {
    fn name(&self) -> &str {
        "Create_Circular_Assoc_Type_Dependency_261"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut assoc_type_metadata = Vec::new();
        let mut type_items = Vec::new();

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(trait_bound) = &impl_item.trait_ {
                    let trait_path = &trait_bound.1;
                    let self_type = &impl_item.self_ty;
                    for impl_item in &mut impl_item.items {
                        if let ImplItem::Type(type_item) = impl_item {
                            assoc_type_metadata.push((trait_path.clone(), self_type.clone(), type_item.ident.clone()));
                            type_items.push(type_item);
                        }
                    }
                }
            }
        }

        if assoc_type_metadata.len() >= 2 && !type_items.is_empty() {
            let mut rng = thread_rng();
            let selected_indices: Vec<_> = (0..assoc_type_metadata.len()).choose_multiple(&mut rng, 2).into_iter().collect();
            let index1 = selected_indices[0];
            let index2 = selected_indices[1];

            let (trait_path1, self_type1, assoc_name1) = &assoc_type_metadata[index1];
            let (trait_path2, self_type2, assoc_name2) = &assoc_type_metadata[index2];
            let type_item1 = &mut type_items[index1];
            let type_item2 = &mut type_items[index2];

            let new_type1 = parse_quote!(<#self_type2 as #trait_path2>::#assoc_name2);
            type_item1.ty = new_type1;

            let new_type2 = parse_quote!(<#self_type1 as #trait_path1>::#assoc_name1);
            type_item2.ty = new_type2;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces circular associated type dependencies between traits by randomly selecting two associated types from different trait implementations and modifying each to reference the other. This creates a cycle in the type system, potentially triggering substitution errors and testing the compiler's ability to resolve cyclic trait bounds and associated type references."
    }
}