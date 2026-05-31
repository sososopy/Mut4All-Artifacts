use syn::parse::{Parse, ParseStream};
use syn::parse_quote;
use syn::punctuated::Punctuated;
use syn::{Meta, MetaList, Attribute, Path, PathArguments};
use proc_macro2::Span;
use quote::ToTokens;
use crate::mutator::Mutator;

pub struct Modify_Enum_Representation_464;

impl Mutator for Modify_Enum_Representation_464 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_464"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut repr_packed = false;
                let mut repr_align = false;

                // Check for repr attributes
                item_enum.attrs.retain(|attr| {
                    if let Some(segment) = attr.path().segments.first() {
                        if segment.ident == "repr" {
                            attr.parse_args_with(|input: ParseStream| {
                                while !input.is_empty() {
                                    let nested: Meta = input.parse()?;
                                    if let Meta::Path(path) = &nested {
                                        if let Some(ident) = path.get_ident() {
                                            if ident == "packed" {
                                                repr_packed = true;
                                            } else if ident.to_string().starts_with("align") {
                                                repr_align = true;
                                            }
                                        }
                                    }
                                    if !input.is_empty() {
                                        input.parse::<syn::Token![,]>()?;
                                    }
                                }
                                Ok(())
                            }).ok();
                        }
                    }
                    true
                });

                // Apply mutation if conditions are met
                if repr_packed && repr_align {
                    // Remove align attribute
                    item_enum.attrs.retain(|attr| {
                        if let Some(segment) = attr.path().segments.first() {
                            if segment.ident == "repr" {
                                let mut new_nested = Punctuated::new();
                                attr.parse_args_with(|input: ParseStream| {
                                    while !input.is_empty() {
                                        let nested: Meta = input.parse()?;
                                        if let Meta::Path(path) = &nested {
                                            if let Some(ident) = path.get_ident() {
                                                if !ident.to_string().starts_with("align") {
                                                    new_nested.push(nested);
                                                }
                                            }
                                        }
                                        if !input.is_empty() {
                                            input.parse::<syn::Token![,]>()?;
                                        }
                                    }
                                    Ok(())
                                }).ok();
                                let new_meta = Meta::List(MetaList {
                                    path: attr.path().clone(),
                                    delimiter: syn::MacroDelimiter::Paren(syn::token::Paren(Span::call_site())),
                                    tokens: new_nested.into_token_stream(),
                                });
                                *attr = Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    meta: new_meta,
                                };
                            }
                        }
                        true
                    });

                    // Add a new variant with fields of different sizes
                    item_enum.variants.push(parse_quote! {
                        VariantWithFields { a: u16, b: u32 }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies enums with `#[repr(packed, align(...))]` attributes, removes the `align` attribute, and adds a new variant with fields of varying sizes. This transformation tests the compiler's handling of enums with complex memory layouts and representation changes, particularly in transmutation contexts."
    }
}