pub struct ADT_Const_Param_Combination_289;

impl Mutator for ADT_Const_Param_Combination_289 {
    fn name(&self) -> &str {
        "ADT_Const_Param_Combination_289"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_adt = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "Adt" {
                    has_adt = true;
                    break;
                }
            }
        }
        if !has_adt {
            let new_adt = parse_quote! {
                pub struct Adt;
            };
            file.items.push(Item::Struct(new_adt));
        }

        let mut has_add_trait = false;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "Add" {
                    has_add_trait = true;
                    break;
                }
            }
        }
        if !has_add_trait {
            let new_trait = parse_quote! {
                pub trait Add<T, const B: bool> {
                }
            };
            file.items.push(Item::Trait(new_trait));
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let has_adt_const = item_struct.generics.params.iter().any(|param| {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            let standard_types = ["i8", "i16", "i32", "i64", "i128", "isize", "u8", "u16", "u32", "u64", "u128", "usize", "f32", "f64", "bool", "char", "str", "String", "Option", "Result", "Vec", "Box", "Arc", "Rc", "Mutex", "RwLock", "Cell", "RefCell", "HashMap", "HashSet", "BTreeMap", "BTreeSet", "VecDeque", "LinkedList", "BinaryHeap", "Path", "PathBuf", "Cow", "Duration", "Instant", "SystemTime", "IoResult", "Error", "Future", "Pin", "BoxFuture", "BoxStream", "Stream", "TryFuture", "TryStream", "Sink", "SinkExt", "StreamExt", "FutureExt", "Pin", "Box", "Arc", "Rc", "Ref", "RefMut", "Cell", "RefCell", "Mutex", "RwLock", "Once", "OnceLock", "Lazy", "LazyLock", "Cow", "Path", "PathBuf", "OsString", "OsStr", "String", "str", "char", "bool", "f32", "f64", "i8", "i16", "i32", "i64", "i128", "isize", "u8", "u16", "u32", "u64", "u128", "usize"];
                            let is_standard = type_path.path.segments.iter().any(|seg| standard_types.contains(&seg.ident.to_string().as_str()));
                            return !is_standard;
                        }
                    }
                    false
                });

                if has_adt_const {
                    let struct_ident = &item_struct.ident;
                    let struct_generics = &item_struct.generics;

                    let mut impl_generics = struct_generics.clone();
                    impl_generics.params.push(parse_quote!(const B: bool));
                    impl_generics.params.push(parse_quote!(T));

                    let trait_path: syn::Path = parse_quote!(Add<T, B>);
                    let struct_path = parse_quote!(#struct_ident<#struct_generics>);

                    let impl_block = parse_quote! {
                        impl<#impl_generics> #trait_path for #struct_path {}
                    };

                    file.items.push(Item::Impl(impl_block));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}