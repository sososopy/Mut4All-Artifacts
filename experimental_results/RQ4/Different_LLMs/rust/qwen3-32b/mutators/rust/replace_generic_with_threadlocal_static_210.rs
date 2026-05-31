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

pub struct Replace_Generic_With_ThreadLocal_Static_210;

impl Mutator for Replace_Generic_With_ThreadLocal_Static_210 {
    fn name(&self) -> &str {
        "Replace_Generic_With_ThreadLocal_Static_210"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &mut *item_impl.self_ty {
                    for segment in &mut type_path.path.segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Type(ty) = arg {
                                    let macro_type: Type = parse_quote! {
                                        thread_local! { pub static X: #ty = #ty { }; }
                                    };
                                    *arg = GenericArgument::Type(macro_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic type arguments in impl blocks with a macro invocation that expands to a thread_local! static item referencing the same type. This creates a mismatch between the macro-generated HIR node's owner and the impl block's owner, leading to ICEs during typeck validation. The transformation stresses the compiler's macro expansion and HIR ownership tracking systems."
    }
}