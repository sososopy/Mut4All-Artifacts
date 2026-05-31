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

pub struct Replace_Return_With_Impl_Trait_Struct_480;

impl Mutator for Replace_Return_With_Impl_Trait_Struct_480 {
    fn name(&self) -> &str {
        "Replace_Return_With_Impl_Trait_Struct_480"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut items_to_insert = vec![];
        let mut struct_counter = 0;
        
        for (i, item) in file.items.iter_mut().enumerate() {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        continue;
                    }
                    
                    let return_type_str = match return_type {
                        Type::Path(p) => {
                            let path = &p.path;
                            let last = path.segments.last().unwrap();
                            last.ident.to_string()
                        }
                        _ => continue,
                    };
                    
                    let alias_name = Ident::new(&format!("Alias{}", rng.gen_range(1000..9999)), Span::call_site());
                    let trait_name = match return_type_str.as_str() {
                        "Vec" => "Iterator",
                        "Option" => "Iterator",
                        "Result" => "Iterator",
                        _ => continue,
                    };
                    
                    let trait_bound = TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: None,
                        path: SynPath::from(Ident::new(trait_name, Span::call_site())),
                    };
                    
                    let bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(trait_bound)]);
                    let impl_trait = TypeImplTrait {
                        impl_token: token::Impl { span: Span::call_site() },
                        bounds,
                    };
                    
                    let alias = Item::Type(syn::ItemType {
                        attrs: vec![],
                        vis: parse_quote!(pub),
                        type_token: Default::default(),
                        ident: alias_name.clone(),
                        gen_type: Box::new(Type::ImplTrait(impl_trait)),
                        semi_token: Some(Default::default()),
                    });
                    
                    items_to_insert.push((i, alias));
                    
                    func.sig.output = ReturnType::Type(
                        parse_quote!(()),
                        Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(alias_name),
                        }))
                    );
                    
                    struct_counter += 1;
                    let struct_name = Ident::new(&format!("DropStruct{}", struct_counter), Span::call_site());
                    let struct_item = Item::Struct(ItemStruct {
                        attrs: vec![],
                        vis: parse_quote!(pub),
                        struct_token: Default::default(),
                        ident: struct_name,
                        generics: parse_quote!(),
                        fields: parse_quote!({ x: #alias_name }),
                        semi_token: None,
                        brace_token: Default::default(),
                    });
                    
                    items_to_insert.push((i + 1, struct_item));
                }
            }
        }
        
        for (pos, item) in items_to_insert.into_iter().rev() {
            file.items.insert(pos, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms concrete return types into opaque impl Trait aliases with iterator bounds, then creates structs holding these values. This forces complex MIR drop handling during monomorphization by requiring DropGlue generation for the opaque type alias, stressing the compiler's type projection and drop logic systems."
    }
}