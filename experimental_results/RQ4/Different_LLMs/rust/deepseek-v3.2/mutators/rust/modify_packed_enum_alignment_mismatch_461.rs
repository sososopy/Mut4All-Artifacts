use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Packed_Enum_Alignment_Mismatch_461;

impl Mutator for Modify_Packed_Enum_Alignment_Mismatch_461 {
    fn name(&self) -> &str {
        "Modify_Packed_Enum_Alignment_Mismatch_461"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut has_packed = false;
                let mut has_repr_c = false;
                let mut alignment_value: Option<u64> = None;
                let mut repr_attrs_to_remove = Vec::new();
                let mut repr_attrs = Vec::new();
                
                for (i, attr) in enum_item.attrs.iter().enumerate() {
                    if attr.path().is_ident("repr") {
                        if let Ok(meta) = attr.parse_args::<syn::Meta>() {
                            match meta {
                                syn::Meta::List(list) => {
                                    let mut found_packed = false;
                                    let mut found_c = false;
                                    let mut current_alignment = None;
                                    
                                    let _ = list.parse_nested_meta(|meta| {
                                        if meta.path.is_ident("C") {
                                            found_c = true;
                                            has_repr_c = true;
                                            Ok(())
                                        } else if meta.path.is_ident("packed") {
                                            found_packed = true;
                                            has_packed = true;
                                            if let Ok(value) = meta.input.parse::<syn::LitInt>() {
                                                if let Ok(parsed_value) = value.base10_parse::<u64>() {
                                                    current_alignment = Some(parsed_value);
                                                    alignment_value = Some(parsed_value);
                                                }
                                            }
                                            Ok(())
                                        } else {
                                            Ok(())
                                        }
                                    });
                                    
                                    if found_packed {
                                        repr_attrs_to_remove.push(i);
                                        repr_attrs.push((found_c, current_alignment));
                                    }
                                }
                                _ => {}
                            }
                        }
                    }
                }
                
                if has_packed && alignment_value.is_some() {
                    let mut rng = thread_rng();
                    let original_alignment = alignment_value.unwrap();
                    let mut new_alignment = original_alignment;
                    
                    if original_alignment.is_power_of_two() {
                        if original_alignment == 1 {
                            new_alignment = 2;
                        } else {
                            new_alignment = original_alignment / 2;
                        }
                    } else {
                        new_alignment = original_alignment.next_power_of_two() / 2;
                    }
                    
                    if new_alignment == 0 {
                        new_alignment = u64::MAX;
                    }
                    
                    if new_alignment > 256 {
                        new_alignment = 256;
                    }
                    
                    for &index in repr_attrs_to_remove.iter().rev() {
                        enum_item.attrs.remove(index);
                    }
                    
                    for (has_c, _) in repr_attrs {
                        let should_add_c = if has_c {
                            rng.gen_bool(0.5)
                        } else {
                            true
                        };
                        
                        let repr_attr = if should_add_c {
                            syn::parse_quote! {
                                #[repr(C, packed(#new_alignment))]
                            }
                        } else {
                            syn::parse_quote! {
                                #[repr(packed(#new_alignment))]
                            }
                        };
                        
                        enum_item.attrs.push(repr_attr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum declarations with #[repr(packed(N))] or #[repr(C, packed(N))] attributes. It modifies the alignment value N to create potential layout inconsistencies: for power-of-two alignments, it changes to the next lower power (or 2 if N=1); for non-power-of-two, it rounds down to nearest power. It also randomly toggles the presence of 'C' in repr attributes. This transformation aims to expose bugs in layout computation, particularly with transmutability features and packed enums, by creating alignment mismatches that stress the compiler's ABI and layout systems."
    }
}