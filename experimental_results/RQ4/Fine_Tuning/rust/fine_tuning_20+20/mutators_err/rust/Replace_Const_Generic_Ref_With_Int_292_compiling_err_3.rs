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

pub struct Replace_Const_Generic_Ref_With_Int_292;

impl Mutator for Replace_Const_Generic_Ref_With_Int_292 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Ref_With_Int_292"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut const_generic_index = None;
                for (i, generic) in func.sig.generics.params.iter().enumerate() {
                    if let syn::GenericParam::Const(const_param) = generic {
                        if let syn::Type::Reference(type_ref) = &*const_param.ty {
                            if let syn::Type::Path(type_path) = &*type_ref.elem {
                                if type_path.path.segments.len() == 1
                                    && type_path.path.segments[0].ident == "MyStr"
                                {
                                    const_generic_index = Some(i);
                                }
                            }
                        }
                    }
                }
                if let Some(index) = const_generic_index {
                    func.sig.generics.params[index] = syn::GenericParam::Const(
                        syn::ConstParam {
                            attrs: Vec::new(),
                            const_token: Default::default(),
                            ident: Ident::new("S", Span::call_site()),
                            colon_token: Default::default(),
                            ty: syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("usize", Span::call_site())),
                            }),
                            eq_token: None,
                            default: None,
                        },
                    );
                    func.sig.output = syn::ReturnType::Type(
                        Default::default(),
                        Box::new(syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("usize", Span::call_site())),
                        })),
                    );
                    func.block = Box::new(syn::parse_quote!({
                        S + 1
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with a constant generic parameter of type reference to `MyStr`. It replaces the constant generic type with `usize`, modifies the return type to `usize`, and updates the function body to return `S + 1`. This transformation stresses the compiler's handling of constant generics and type inference, potentially exposing issues in constant evaluation and type resolution."
    }
}