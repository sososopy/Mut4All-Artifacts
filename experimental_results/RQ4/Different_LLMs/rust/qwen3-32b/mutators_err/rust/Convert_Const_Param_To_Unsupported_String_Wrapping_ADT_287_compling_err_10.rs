use syn::parse_quote;
use crate::mutator::Mutator;
use syn::GenericParam;
use syn::Type;
use syn::Item;
use syn::TypeReference;
use syn::TypePath;
use syn::token::And;
use syn::ImplItem;

pub struct Convert_Const_Param_To_Unsupported_String_Wrapping_ADT_287;

impl Mutator for Convert_Const_Param_To_Unsupported_String_Wrapping_ADT_287 {
    fn name(&self) -> &str {
        "Convert_Const_Param_To_Unsupported_String_Wrapping_ADT_287"
    }
    fn mutate(&self, file: &mut syn::File) {
        let has_my_str = file.items.iter().any(|item| {
            if let Item::Struct(item_struct) = item {
                item_struct.ident == "MyStr" && 
                match &item_struct.fields {
                    syn::Fields::Unnamed(fields) => {
                        fields.unnamed.len() == 1 && 
                        match &fields.unnamed[0].ty {
                            Type::Path(type_path) => {
                                type_path.path.is_ident("str") && 
                                type_path.path.leading_colon.is_some()
                            },
                            _ => false,
                        }
                    },
                    _ => false,
                }
            } else {
                false
            }
        });

        let has_const_param_ty = file.items.iter().any(|item| {
            if let Item::Impl(item_impl) = item {
                if let Some(trait_path) = &item_impl.trait_ {
                    if let Some(seg) = trait_path.1.segments.first() {
                        seg.ident == "ConstParamTy" && 
                        match &*item_impl.self_ty {
                            Type::Path(type_path) => type_path.path.is_ident("MyStr"),
                            _ => false,
                        }
                    } else {
                        false
                    }
                } else {
                    false
                }
            } else {
                false
            }
        });

        let has_new_method = file.items.iter().any(|item| {
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    type_path.path.is_ident("MyStr") && 
                    item_impl.items.iter().any(|item| {
                        if let syn::ImplItem::Fn(method) = item {
                            method.sig.ident == "new"
                        } else {
                            false
                        }
                    })
                } else {
                    false
                }
            } else {
                false
            }
        });

        let mut new_items = Vec::new();

        if !has_my_str {
            let new_struct = parse_quote! {
                struct MyStr(str);
            };
            new_items.push(Item::Struct(new_struct));
        }

        if !has_const_param_ty {
            let new_impl = parse_quote! {
                impl ConstParamTy for MyStr {}
            };
            new_items.push(Item::Impl(new_impl));
        }

        if !has_new_method {
            let new_method = parse_quote! {
                impl MyStr {
                    fn new(s: &str) -> &Self {
                        unsafe { std::mem::transmute(s) }
                    }
                }
            };
            new_items.push(Item::Impl(new_method));
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut target_param: Option<&mut GenericParam> = None;
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let is_string_type = match &const_param.ty {
                            Type::Reference(ref_ty) => {
                                matches!(&*ref_ty.elem, Type::Path(path_ty) if path_ty.path.is_ident("str"))
                            },
                            Type::Path(path_ty) => {
                                path_ty.path.is_ident("String")
                            },
                            _ => false,
                        };
                        if !is_string_type {
                            target_param = Some(param);
                            break;
                        }
                    }
                }
                if let Some(param) = target_param {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Type::Reference(TypeReference {
                            and_token: And::default(),
                            lifetime: None,
                            mutability: None,
                            elem: Box::new(Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!("MyStr"),
                            })),
                        });
                    }
                }
            }
        }

        for item in new_items.into_iter().rev() {
            file.items.insert(0, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms a generic const parameter from a supported type (e.g., u32, bool) into a reference to a custom ADT (MyStr) that wraps &str. This introduces an unsupported type in const generics, leveraging unsafe transmutation to bypass type safety. The transformation modifies the function signature, introduces a new ADT, and forces the use of unsafe code to satisfy the compiler's type expectations. This tests the compiler's handling of const generics with non-primitive ADTs and its ability to resolve transmuted references in generic contexts."
    }
}