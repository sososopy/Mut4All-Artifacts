use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Add_Iter_Item_Generic_213;

impl Mutator for Add_Iter_Item_Generic_213 {
    fn name(&self) -> &str {
        "Add_Iter_Item_Generic_213"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.process_function(func);
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        self.process_function(method);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds explicit but redundant generic parameters to iterator return types in associated items. It targets methods returning std::slice::Iter or IterMut where the element type is inferable from the receiver. By appending <Item = T> to the return type, it forces the compiler to process explicit generics where inference would suffice, potentially exposing bugs in metadata handling and associated item resolution."
    }
}

trait HasReturnType {
    fn return_type_mut(&mut self) -> &mut ReturnType;
}

impl HasReturnType for ItemFn {
    fn return_type_mut(&mut self) -> &mut ReturnType {
        &mut self.sig.output
    }
}

impl HasReturnType for ImplItemFn {
    fn return_type_mut(&mut self) -> &mut ReturnType {
        &mut self.sig.output
    }
}

impl Add_Iter_Item_Generic_213 {
    fn process_function<F: HasReturnType>(&self, func: &mut F) {
        if let ReturnType::Type(_, return_type) = func.return_type_mut() {
            if let Type::Path(type_path) = &mut **return_type {
                if self.is_slice_iter_path(&type_path.path) {
                    if let Some(element_ident) = self.get_receiver_element_type(func) {
                        self.modify_return_type_with_item(type_path, &element_ident);
                    }
                }
            }
        }
    }

    fn is_slice_iter_path(&self, path: &SynPath) -> bool {
        path.segments.len() == 3 &&
        path.segments[0].ident == "std" &&
        path.segments[1].ident == "slice" &&
        (path.segments[2].ident == "Iter" || path.segments[2].ident == "IterMut")
    }

    fn get_receiver_element_type(&self, func: &impl FnLike) -> Option<Ident> {
        if let Some(first_arg) = func.sig().inputs.first() {
            if let FnArg::Typed(pat_type) = first_arg {
                if let Type::Reference(type_ref) = &*pat_type.ty {
                    let elem_type = &*type_ref.elem;
                    if let Type::Slice(type_slice) = elem_type {
                        if let Type::Path(type_path) = &*type_slice.elem {
                            if type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                return Some(Ident::new(segment.ident.to_string().as_str(), segment.ident.span()));
                            }
                        }
                    } else if let Type::Array(type_array) = elem_type {
                        if let Type::Path(type_path) = &*type_array.elem {
                            if type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                return Some(Ident::new(segment.ident.to_string().as_str(), segment.ident.span()));
                            }
                        }
                    }
                }
            }
        }
        None
    }

    fn modify_return_type_with_item(&self, type_path: &mut TypePath, element_ident: &Ident) {
        let last_segment = type_path.path.segments.last_mut().unwrap();
        let mut args = Punctuated::new();
        let arg = GenericArgument::AssocType(syn::AssocType {
            eq_token: token::Eq::default(),
            ident: Ident::new("Item", proc_macro2::Span::call_site()),
            ty: Type::Path(TypePath {
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: {
                        let mut segs = Punctuated::new();
                        segs.push(PathSegment {
                            ident: element_ident.clone(),
                            arguments: PathArguments::None,
                        });
                        segs
                    },
                },
            }),
        });
        args.push(arg);
        last_segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
            colon2_token: None,
            lt_token: token::Lt::default(),
            args,
            gt_token: token::Gt::default(),
        });
    }
}

trait FnLike {
    fn sig(&self) -> &syn::Signature;
    fn sig_mut(&mut self) -> &mut syn::Signature;
}

impl FnLike for ItemFn {
    fn sig(&self) -> &syn::Signature {
        &self.sig
    }
    fn sig_mut(&mut self) -> &mut syn::Signature {
        &mut self.sig
    }
}

impl FnLike for ImplItemFn {
    fn sig(&self) -> &syn::Signature {
        &self.sig
    }
    fn sig_mut(&mut self) -> &mut syn::Signature {
        &mut self.sig
    }
}