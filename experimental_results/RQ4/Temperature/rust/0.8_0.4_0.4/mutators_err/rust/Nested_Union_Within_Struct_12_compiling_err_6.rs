use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Nested_Union_Within_Struct_12;

impl Mutator for Nested_Union_Within_Struct_12 {
    fn name(&self) -> &str {
        "Nested_Union_Within_Struct_12"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let existing_structs_and_unions: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Struct(item_struct) => Some(item_struct.ident.clone()),
                Item::Union(item_union) => Some(item_union.ident.clone()),
                _ => None,
            })
            .collect();

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let union_name = Ident::new("nested_union", Span::call_site());
                let field1_name = Ident::new("field1", Span::call_site());
                let field2_name = Ident::new("field2", Span::call_site());

                let field2_type = if !existing_structs_and_unions.is_empty() {
                    existing_structs_and_unions
                        .choose(&mut rng)
                        .map(|ident| Type::Path(TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: ident.clone(),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }))
                        .unwrap()
                } else {
                    Type::Path(TypePath {
                        qself: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![PathSegment {
                                ident: Ident::new("i32", Span::call_site()),
                                arguments: PathArguments::None,
                            }]),
                        },
                    })
                };

                let new_union = Item::Union(ItemUnion {
                    attrs: Vec::new(),
                    vis: Visibility::Inherited,
                    union_token: token::Union {
                        span: Span::call_site(),
                    },
                    ident: union_name.clone(),
                    generics: Generics::default(),
                    fields: FieldsNamed {
                        brace_token: token::Brace {
                            span: Span::call_site(),
                        },
                        named: {
                            let mut fields = Punctuated::new();
                            fields.push(Field {
                                attrs: Vec::new(),
                                vis: Visibility::Inherited,
                                ident: Some(field1_name),
                                colon_token: Some(token::Colon {
                                    spans: [Span::call_site()],
                                }),
                                ty: Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("u8", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                }),
                                mutability: FieldMutability::None,
                            });
                            fields.push(Field {
                                attrs: Vec::new(),
                                vis: Visibility::Inherited,
                                ident: Some(field2_name),
                                colon_token: Some(token::Colon {
                                    spans: [Span::call_site()],
                                }),
                                ty: field2_type,
                                mutability: FieldMutability::None,
                            });
                            fields
                        },
                    },
                });

                let new_union_field = Field {
                    attrs: Vec::new(),
                    vis: Visibility::Inherited,
                    ident: Some(union_name),
                    colon_token: Some(token::Colon {
                        spans: [Span::call_site()],
                    }),
                    ty: Type::Verbatim(quote! { #new_union }),
                    mutability: FieldMutability::None,
                };

                let insert_position = rng.gen_range(0..=item_struct.fields.len());
                match &mut item_struct.fields {
                    Fields::Named(fields_named) => {
                        fields_named.named.insert(insert_position, new_union_field);
                    }
                    Fields::Unnamed(fields_unnamed) => {
                        fields_unnamed.unnamed.insert(insert_position, new_union_field);
                    }
                    Fields::Unit => {}
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs in the seed program and introduces a nested union within them. The union contains a field of a basic data type and another field using an existing struct or union. This transformation aims to test the compiler's handling of nested anonymous data types within structs, exposing potential issues in type resolution and memory layout management."
    }
}