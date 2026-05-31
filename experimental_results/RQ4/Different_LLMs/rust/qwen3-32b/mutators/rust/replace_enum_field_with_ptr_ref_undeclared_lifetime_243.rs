use crate::mutator::Mutator;
use syn::Item;
use syn::Fields;
use syn::Lifetime;
use proc_macro2::Span;
use syn::Type;
use syn::TypeReference;
use syn::TypePtr;
use syn::token::And;
use syn::token::Const;
use syn::token::Star;

pub struct Replace_Enum_Field_With_Ptr_Ref_Undeclared_Lifetime_243;

impl Mutator for Replace_Enum_Field_With_Ptr_Ref_Undeclared_Lifetime_243 {
    fn name(&self) -> &str {
        "Replace_Enum_Field_With_Ptr_Ref_Undeclared_Lifetime_243"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    match &mut variant.fields {
                        Fields::Named(fields) => {
                            for field in fields.named.iter_mut() {
                                let original_type = field.ty.clone();
                                let lifetime = Lifetime::new("'a", Span::call_site());
                                let reference_type = Type::Reference(TypeReference {
                                    and_token: And::default(),
                                    lifetime: Some(lifetime),
                                    mutability: None,
                                    elem: Box::new(original_type),
                                });
                                let pointer_type = Type::Ptr(TypePtr {
                                    const_token: Some(Const::default()),
                                    star_token: Star::default(),
                                    mutability: None,
                                    elem: Box::new(reference_type),
                                });
                                field.ty = pointer_type;
                            }
                        }
                        Fields::Unnamed(fields) => {
                            for field in fields.unnamed.iter_mut() {
                                let original_type = field.ty.clone();
                                let lifetime = Lifetime::new("'a", Span::call_site());
                                let reference_type = Type::Reference(TypeReference {
                                    and_token: And::default(),
                                    lifetime: Some(lifetime),
                                    mutability: None,
                                    elem: Box::new(original_type),
                                });
                                let pointer_type = Type::Ptr(TypePtr {
                                    const_token: Some(Const::default()),
                                    star_token: Star::default(),
                                    mutability: None,
                                    elem: Box::new(reference_type),
                                });
                                field.ty = pointer_type;
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces enum variant field types with a raw pointer to a reference using an undeclared lifetime parameter 'a. This transformation introduces invalid types with undeclared lifetimes, testing the compiler's ability to handle raw pointer/ref interactions and undeclared lifetime usage in pattern matching contexts."
    }
}