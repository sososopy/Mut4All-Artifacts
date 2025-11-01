use crate::mutator::Mutator;
use syn::{File, Item, Fields, Type, Stmt, Expr, Pat};
use proc_macro2::Ident;

pub struct Enum_Lifetime_Mismatch_364;

impl Mutator for Enum_Lifetime_Mismatch_364 {
    fn name(&self) -> &str {
        "Enum_Lifetime_Mismatch_364"
    }

    fn mutate(&self, file: &mut File) {
        let mut has_lifetime_variant = false;
        let mut enum_ident = None;

        for item in &file.items {
            if let Item::Enum(item_enum) = item {
                for variant in &item_enum.variants {
                    if let Fields::Named(fields_named) = &variant.fields {
                        for field in &fields_named.named {
                            if let Type::Reference(type_ref) = &field.ty {
                                if type_ref.lifetime.is_some() {
                                    has_lifetime_variant = true;
                                    enum_ident = Some(item_enum.ident.clone());
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if has_lifetime_variant {
            if let Some(enum_ident) = enum_ident {
                for item in &mut file.items {
                    if let Item::Fn(item_fn) = item {
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Expr(Expr::Match(expr_match)) = stmt {
                                for arm in &mut expr_match.arms {
                                    if let Pat::TupleStruct(pat_tuple_struct) = &mut arm.pat {
                                        if pat_tuple_struct.path.segments.len() == 1 {
                                            let segment = &mut pat_tuple_struct.path.segments[0];
                                            if segment.ident == enum_ident {
                                                segment.ident = Ident::new("NonExistentVariant", segment.ident.span());
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants with lifetimes. It identifies enums with variants that include pointer or reference types with lifetimes. It then mutates pattern matching statements to introduce a type or lifetime mismatch by changing the matched pattern to a non-existent variant. This exploits potential mismanagement of lifetimes and pattern matching in Rust, which can lead to internal compiler errors or unexpected behavior."
    }
}