use syn::{Type, TypeReference, TypeTuple, Lifetime, Fields, Item, ItemEnum, Variant, Field, Token, visit::Mutator};
use proc_macro2::Span;
use syn::punctuated::Punctuated;

struct EnumVariantTypeMismatch_243;

impl Mutator for EnumVariantTypeMismatch_243 {
    fn name(&self) -> &str {
        "EnumVariantTypeMismatch_243"
    }

    fn visit_item_enum(&mut self, node: &mut ItemEnum) {
        for variant in &mut node.variants {
            if let Fields::Named(fields) = &mut variant.fields {
                for field in &mut fields.named {
                    if let Type::Reference(ref ty) = &field.ty {
                        let new_ty = Type::Reference(TypeReference {
                            and_token: ty.and_token.clone(),
                            lifetime: Some(Lifetime::new("static", Span::call_site())),
                            mutability: ty.mutability,
                            elem: Box::new(Type::Tuple(TypeTuple {
                                paren_token: syn::token::Paren::default(),
                                elems: Punctuated::new(),
                            })),
                        });
                        field.ty = new_ty;
                    } else {
                        field.ty = Type::Tuple(TypeTuple {
                            paren_token: syn::token::Paren::default(),
                            elems: Punctuated::new(),
                        });
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants with type annotations and introduces a type mismatch between the declared type of the variant and the type used in a pattern match. This transformation aims to test the compiler's ability to handle type checking and pattern matching on enum variants with complex types."
    }
}