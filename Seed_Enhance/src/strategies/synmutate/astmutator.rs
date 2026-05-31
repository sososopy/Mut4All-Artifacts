use std::{collections::HashMap, hash::Hash};

use quote::ToTokens;
use syn::visit_mut::VisitMut;

use indexmap::IndexMap;

use crate::{util::glob_range, warn};

// γ
const CHOOSE_ADJ_RATE: f64 = 0.95;
// w_lb
const MIN_CHOOSE: f64 = 0.3;
// T
const MAX_ANALYZE_DEPTH: usize = 200;

// Note: The base code is copied from syn::visit_mut::VisitMut
// and modified automatically

#[derive(PartialEq, Eq, Hash, Clone)]
pub(crate) enum ASTStore {
    Abi(syn::Abi),
    AngleBracketedGenericArguments(syn::AngleBracketedGenericArguments),
    Arm(syn::Arm),
    AssocConst(syn::AssocConst),
    AssocType(syn::AssocType),
    AttrStyle(syn::AttrStyle),
    Attribute(syn::Attribute),
    VecAttribute(Vec<syn::Attribute>),
    BareFnArg(syn::BareFnArg),
    BareVariadic(syn::BareVariadic),
    BinOp(syn::BinOp),
    Block(syn::Block),
    BoundLifetimes(syn::BoundLifetimes),
    CapturedParam(syn::CapturedParam),
    ConstParam(syn::ConstParam),
    Constraint(syn::Constraint),
    Data(syn::Data),
    DataEnum(syn::DataEnum),
    DataStruct(syn::DataStruct),
    DataUnion(syn::DataUnion),
    DeriveInput(syn::DeriveInput),
    Expr(syn::Expr),
    ExprArray(syn::ExprArray),
    ExprAssign(syn::ExprAssign),
    ExprAsync(syn::ExprAsync),
    ExprAwait(syn::ExprAwait),
    ExprBinary(syn::ExprBinary),
    ExprBlock(syn::ExprBlock),
    ExprBreak(syn::ExprBreak),
    ExprCall(syn::ExprCall),
    ExprCast(syn::ExprCast),
    ExprClosure(syn::ExprClosure),
    ExprConst(syn::ExprConst),
    ExprContinue(syn::ExprContinue),
    ExprField(syn::ExprField),
    ExprForLoop(syn::ExprForLoop),
    ExprGroup(syn::ExprGroup),
    ExprIf(syn::ExprIf),
    ExprIndex(syn::ExprIndex),
    ExprInfer(syn::ExprInfer),
    ExprLet(syn::ExprLet),
    ExprLit(syn::ExprLit),
    ExprLoop(syn::ExprLoop),
    ExprMacro(syn::ExprMacro),
    ExprMatch(syn::ExprMatch),
    ExprMethodCall(syn::ExprMethodCall),
    ExprParen(syn::ExprParen),
    ExprPath(syn::ExprPath),
    ExprRange(syn::ExprRange),
    ExprRawAddr(syn::ExprRawAddr),
    ExprReference(syn::ExprReference),
    ExprRepeat(syn::ExprRepeat),
    ExprReturn(syn::ExprReturn),
    ExprStruct(syn::ExprStruct),
    ExprTry(syn::ExprTry),
    ExprTryBlock(syn::ExprTryBlock),
    ExprTuple(syn::ExprTuple),
    ExprUnary(syn::ExprUnary),
    ExprUnsafe(syn::ExprUnsafe),
    ExprWhile(syn::ExprWhile),
    ExprYield(syn::ExprYield),
    Field(syn::Field),
    FieldMutability(syn::FieldMutability),
    FieldPat(syn::FieldPat),
    FieldValue(syn::FieldValue),
    Fields(syn::Fields),
    FieldsNamed(syn::FieldsNamed),
    FieldsUnnamed(syn::FieldsUnnamed),
    File(syn::File),
    FnArg(syn::FnArg),
    ForeignItem(syn::ForeignItem),
    ForeignItemFn(syn::ForeignItemFn),
    ForeignItemMacro(syn::ForeignItemMacro),
    ForeignItemStatic(syn::ForeignItemStatic),
    ForeignItemType(syn::ForeignItemType),
    GenericArgument(syn::GenericArgument),
    GenericParam(syn::GenericParam),
    Generics(syn::Generics),
    Ident(proc_macro2::Ident),
    ImplItem(syn::ImplItem),
    ImplItemConst(syn::ImplItemConst),
    ImplItemFn(syn::ImplItemFn),
    ImplItemMacro(syn::ImplItemMacro),
    ImplItemType(syn::ImplItemType),
    ImplRestriction(syn::ImplRestriction),
    Index(syn::Index),
    Item(syn::Item),
    ItemConst(syn::ItemConst),
    ItemEnum(syn::ItemEnum),
    ItemFn(syn::ItemFn),
    ItemForeignMod(syn::ItemForeignMod),
    ItemImpl(syn::ItemImpl),
    ItemMacro(syn::ItemMacro),
    ItemMod(syn::ItemMod),
    ItemStatic(syn::ItemStatic),
    ItemStruct(syn::ItemStruct),
    ItemTrait(syn::ItemTrait),
    ItemTraitAlias(syn::ItemTraitAlias),
    ItemType(syn::ItemType),
    ItemUnion(syn::ItemUnion),
    ItemUse(syn::ItemUse),
    Label(syn::Label),
    Lifetime(syn::Lifetime),
    LifetimeParam(syn::LifetimeParam),
    Lit(syn::Lit),
    LitBool(syn::LitBool),
    LitByte(syn::LitByte),
    LitByteStr(syn::LitByteStr),
    LitCStr(syn::LitCStr),
    LitChar(syn::LitChar),
    LitFloat(syn::LitFloat),
    LitInt(syn::LitInt),
    LitStr(syn::LitStr),
    Local(syn::Local),
    LocalInit(syn::LocalInit),
    Macro(syn::Macro),
    MacroDelimiter(syn::MacroDelimiter),
    Member(syn::Member),
    Meta(syn::Meta),
    MetaList(syn::MetaList),
    MetaNameValue(syn::MetaNameValue),
    Pat(syn::Pat),
    PatIdent(syn::PatIdent),
    PatOr(syn::PatOr),
    PatParen(syn::PatParen),
    PatReference(syn::PatReference),
    PatRest(syn::PatRest),
    PatSlice(syn::PatSlice),
    PatStruct(syn::PatStruct),
    PatTuple(syn::PatTuple),
    PatTupleStruct(syn::PatTupleStruct),
    PatType(syn::PatType),
    PatWild(syn::PatWild),
    Path(syn::Path),
    PathArguments(syn::PathArguments),
    PathSegment(syn::PathSegment),
    PointerMutability(syn::PointerMutability),
    PreciseCapture(syn::PreciseCapture),
    PredicateLifetime(syn::PredicateLifetime),
    PredicateType(syn::PredicateType),
    QSelf(syn::QSelf),
    RangeLimits(syn::RangeLimits),
    Receiver(syn::Receiver),
    ReturnType(syn::ReturnType),
    Signature(syn::Signature),
    StaticMutability(syn::StaticMutability),
    Stmt(syn::Stmt),
    StmtMacro(syn::StmtMacro),
    TraitBound(syn::TraitBound),
    TraitBoundModifier(syn::TraitBoundModifier),
    TraitItem(syn::TraitItem),
    TraitItemConst(syn::TraitItemConst),
    TraitItemFn(syn::TraitItemFn),
    TraitItemMacro(syn::TraitItemMacro),
    TraitItemType(syn::TraitItemType),
    Type(syn::Type),
    TypeArray(syn::TypeArray),
    TypeBareFn(syn::TypeBareFn),
    TypeGroup(syn::TypeGroup),
    TypeImplTrait(syn::TypeImplTrait),
    TypeInfer(syn::TypeInfer),
    TypeMacro(syn::TypeMacro),
    TypeNever(syn::TypeNever),
    TypeParam(syn::TypeParam),
    TypeParamBound(syn::TypeParamBound),
    TypeParen(syn::TypeParen),
    TypePath(syn::TypePath),
    TypePtr(syn::TypePtr),
    TypeReference(syn::TypeReference),
    TypeSlice(syn::TypeSlice),
    TypeTraitObject(syn::TypeTraitObject),
    TypeTuple(syn::TypeTuple),
    UnOp(syn::UnOp),
    UseGlob(syn::UseGlob),
    UseGroup(syn::UseGroup),
    UseName(syn::UseName),
    UsePath(syn::UsePath),
    UseRename(syn::UseRename),
    UseTree(syn::UseTree),
    Variadic(syn::Variadic),
    Variant(syn::Variant),
    VisRestricted(syn::VisRestricted),
    Visibility(syn::Visibility),
    WhereClause(syn::WhereClause),
    WherePredicate(syn::WherePredicate),
}

const SKIPS: [&str; 1] = [
    // We skip these nodes, as they are tooo big to store
    "syn::File",
];

macro_rules! gen_weight {
    (AttrStyle, $nd: ident) => {
        1.
    };
    (VecAttribute, $nd: ident) => {
        1.
    };
    (Data, $nd: ident) => {
        1.
    };
    (DataEnum, $nd: ident) => {
        1.
    };
    (DataStruct, $nd: ident) => {
        1.
    };
    (DataUnion, $nd: ident) => {
        1.
    };
    (FieldMutability, $nd: ident) => {
        1.
    };
    (ImplRestriction, $nd: ident) => {
        1.
    };
    (LocalInit, $nd: ident) => {
        1.
    };
    (MacroDelimiter, $nd: ident) => {
        1.
    };
    (QSelf, $nd: ident) => {
        1.
    };
    ($nd_ty_store: ident, $nd: ident) => {{
        let llen = $nd.to_token_stream().to_string().len() as f64;
        if llen < 100. {
            1.
        } else {
            0.7 / (llen - 100. + 1.) + MIN_CHOOSE
        }
    }};
}

macro_rules! do_work {
    ($self: ident, $nd_name:expr, $nd_ty: ty, $nd_ty_store: ident, $nd: ident) => {
        'end_work: {
            if SKIPS.contains(&$nd_name) {
                break 'end_work;
            }
            if $self.analyze_depth >= MAX_ANALYZE_DEPTH {
                // Some case intentionally make the compiler stack overflow
                // So we need to limit the analyze depth
                return;
            }
            // crate::info!("Depth: {}", $self.analyze_depth);
            // crate::info!("Sz: {}", std::mem::size_of::<Self>());
            match $self.work_mode {
                WorkMode::Add => {
                    let ast_store = ASTStore::$nd_ty_store($nd.clone());
                    let store_place = match $self.nodeset.get_mut($nd_name) {
                        Some(v) => v,
                        None => {
                            $self.nodeset.insert($nd_name.to_string(), (IndexMap::new(), 0.));
                            $self.nodeset.get_mut($nd_name).unwrap()
                        }
                    };
                    let w = gen_weight!($nd_ty_store, $nd);
                    store_place.1 += w;
                    store_place.0.insert(ast_store, w);
                }
                WorkMode::Modify => {
                    if $self.nested >= 30 {
                        break 'end_work;
                    }
                    if glob_range(0. ..1.) > 0.1 {
                        break 'end_work;
                    }
                    let store_place = match $self.nodeset.get_mut($nd_name) {
                        Some(v) => v,
                        None => {
                            $self.nodeset.insert($nd_name.to_string(), (IndexMap::new(), 0.));
                            $self.nodeset.get_mut($nd_name).unwrap()
                        }
                    };
                    if store_place.0.is_empty() {
                        break 'end_work;
                    }
                    let w = glob_range(0. ..store_place.1);
                    // let ast_store = store_place.get_index(idx).unwrap();
                    let mut cur_w = 0.;
                    let ast_store = store_place.0
                        .iter_mut()
                        .find(|(_, v)| {
                            cur_w += **v;
                            cur_w >= w
                        });
                    let ast_store = match ast_store {
                        Some(ast_store) => ast_store,
                        None => {
                            store_place.0
                                .last_mut()
                                .unwrap_or_else(|| panic!("No node found for syn::ABI"))
                        }
                    };
        
                    let node = match ast_store.0 {
                        ASTStore::$nd_ty_store(expr) => expr,
                        _ => unreachable!(),
                    };
                    let node = node.clone();
                    *$nd = node;
                    $self.nested += 1;
                    let new_val = *ast_store.1 * CHOOSE_ADJ_RATE;
                    if new_val > MIN_CHOOSE {
                        store_place.1 -= (*ast_store.1 - new_val);
                        *ast_store.1 = new_val;
                    }
                }
                WorkMode::Adjust(dup) => {
                    let ast_store = ASTStore::$nd_ty_store($nd.clone());
                    let store_place = match $self.nodeset.get_mut($nd_name) {
                        Some(v) => v,
                        None => {
                            $self.nodeset.insert($nd_name.to_string(), (IndexMap::new(), 0.));
                            $self.nodeset.get_mut($nd_name).unwrap()
                        }
                    };
                    let res = store_place.0.get_mut(&ast_store);
                    match res {
                        Some(x) => {
                            let adj = *x * {
                                if dup {
                                    0.95
                                } else {
                                    1.05
                                }
                            };
                            store_place.1 -= (*x - adj);
                            *x = adj;
                        }
                        None => {}
                    }
                }
            }
        }
    };
}

#[derive(Clone)]
enum WorkMode {
    Add,
    Modify,
    Adjust(bool),
}

#[derive(Clone)]
pub(crate) struct ASTMutator {
    pub nodeset: HashMap<String, (IndexMap<ASTStore, f64>, f64)>,
    work_mode: WorkMode,
    nested: usize,
    analyze_depth: usize,
}
unsafe impl Send for ASTMutator {} // Data won't be cloned outside of the thread,
unsafe impl Sync for ASTMutator {} // at least I think so?

impl ASTMutator {
    pub fn new() -> Self {
        Self {
            nodeset: HashMap::new(),
            work_mode: WorkMode::Add,
            nested: 0,
            analyze_depth: 0,
        }
    }
    pub fn begin_add(&mut self) {
        self.work_mode = WorkMode::Add;
    }
    pub fn begin_modify(&mut self) {
        self.work_mode = WorkMode::Modify;
    }
    pub fn begin_adjust(&mut self, is_dup: bool) {
        self.work_mode = WorkMode::Adjust(is_dup);
    }
}
impl VisitMut for ASTMutator {
    fn visit_abi_mut(&mut self, i: &mut syn::Abi) {
        do_work!(self, "syn::Abi", syn::Abi, Abi, i);
        self.analyze_depth += 1;
        syn::visit_mut::visit_abi_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_angle_bracketed_generic_arguments_mut(
        &mut self,
        i: &mut syn::AngleBracketedGenericArguments,
    ) {
        do_work!(
            self,
            "syn::AngleBracketedGenericArguments",
            syn::AngleBracketedGenericArguments,
            AngleBracketedGenericArguments,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_angle_bracketed_generic_arguments_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_arm_mut(&mut self, i: &mut syn::Arm) {
        do_work!(self, "syn::Arm", syn::Arm, Arm, i);
        self.analyze_depth += 1;
        syn::visit_mut::visit_arm_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_assoc_const_mut(&mut self, i: &mut syn::AssocConst) {
        do_work!(self, "syn::AssocConst", syn::AssocConst, AssocConst, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_assoc_const_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_assoc_type_mut(&mut self, i: &mut syn::AssocType) {
        do_work!(self, "syn::AssocType", syn::AssocType, AssocType, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_assoc_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_attr_style_mut(&mut self, i: &mut syn::AttrStyle) {
        do_work!(self, "syn::AttrStyle", syn::AttrStyle, AttrStyle, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_attr_style_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_attribute_mut(&mut self, i: &mut syn::Attribute) {
        do_work!(self, "syn::Attribute", syn::Attribute, Attribute, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_attribute_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_attributes_mut(&mut self, i: &mut Vec<syn::Attribute>) {
        do_work!(
            self,
            "Vec<syn::Attribute>",
            Vec<syn::Attribute>,
            VecAttribute,
            i
        );

        for attr in i {
            self.visit_attribute_mut(attr);
        }
    }

    fn visit_bare_fn_arg_mut(&mut self, i: &mut syn::BareFnArg) {
        do_work!(self, "syn::BareFnArg", syn::BareFnArg, BareFnArg, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_bare_fn_arg_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_bare_variadic_mut(&mut self, i: &mut syn::BareVariadic) {
        do_work!(
            self,
            "syn::BareVariadic",
            syn::BareVariadic,
            BareVariadic,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_bare_variadic_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_bin_op_mut(&mut self, i: &mut syn::BinOp) {
        do_work!(self, "syn::BinOp", syn::BinOp, BinOp, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_bin_op_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_block_mut(&mut self, i: &mut syn::Block) {
        do_work!(self, "syn::Block", syn::Block, Block, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_block_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_bound_lifetimes_mut(&mut self, i: &mut syn::BoundLifetimes) {
        do_work!(
            self,
            "syn::BoundLifetimes",
            syn::BoundLifetimes,
            BoundLifetimes,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_bound_lifetimes_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_captured_param_mut(&mut self, i: &mut syn::CapturedParam) {
        do_work!(
            self,
            "syn::CapturedParam",
            syn::CapturedParam,
            CapturedParam,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_captured_param_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_const_param_mut(&mut self, i: &mut syn::ConstParam) {
        do_work!(self, "syn::ConstParam", syn::ConstParam, ConstParam, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_const_param_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_constraint_mut(&mut self, i: &mut syn::Constraint) {
        do_work!(self, "syn::Constraint", syn::Constraint, Constraint, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_constraint_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_data_mut(&mut self, i: &mut syn::Data) {
        do_work!(self, "syn::Data", syn::Data, Data, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_data_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_data_enum_mut(&mut self, i: &mut syn::DataEnum) {
        do_work!(self, "syn::DataEnum", syn::DataEnum, DataEnum, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_data_enum_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_data_struct_mut(&mut self, i: &mut syn::DataStruct) {
        do_work!(self, "syn::DataStruct", syn::DataStruct, DataStruct, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_data_struct_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_data_union_mut(&mut self, i: &mut syn::DataUnion) {
        do_work!(self, "syn::DataUnion", syn::DataUnion, DataUnion, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_data_union_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_derive_input_mut(&mut self, i: &mut syn::DeriveInput) {
        do_work!(self, "syn::DeriveInput", syn::DeriveInput, DeriveInput, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_derive_input_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_mut(&mut self, i: &mut syn::Expr) {
        do_work!(self, "syn::Expr", syn::Expr, Expr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_array_mut(&mut self, i: &mut syn::ExprArray) {
        do_work!(self, "syn::ExprArray", syn::ExprArray, ExprArray, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_array_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_assign_mut(&mut self, i: &mut syn::ExprAssign) {
        do_work!(self, "syn::ExprAssign", syn::ExprAssign, ExprAssign, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_assign_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_async_mut(&mut self, i: &mut syn::ExprAsync) {
        do_work!(self, "syn::ExprAsync", syn::ExprAsync, ExprAsync, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_async_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_await_mut(&mut self, i: &mut syn::ExprAwait) {
        do_work!(self, "syn::ExprAwait", syn::ExprAwait, ExprAwait, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_await_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_binary_mut(&mut self, i: &mut syn::ExprBinary) {
        do_work!(self, "syn::ExprBinary", syn::ExprBinary, ExprBinary, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_binary_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_block_mut(&mut self, i: &mut syn::ExprBlock) {
        do_work!(self, "syn::ExprBlock", syn::ExprBlock, ExprBlock, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_block_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_break_mut(&mut self, i: &mut syn::ExprBreak) {
        do_work!(self, "syn::ExprBreak", syn::ExprBreak, ExprBreak, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_break_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_call_mut(&mut self, i: &mut syn::ExprCall) {
        do_work!(self, "syn::ExprCall", syn::ExprCall, ExprCall, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_call_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_cast_mut(&mut self, i: &mut syn::ExprCast) {
        do_work!(self, "syn::ExprCast", syn::ExprCast, ExprCast, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_cast_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_closure_mut(&mut self, i: &mut syn::ExprClosure) {
        do_work!(self, "syn::ExprClosure", syn::ExprClosure, ExprClosure, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_closure_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_const_mut(&mut self, i: &mut syn::ExprConst) {
        do_work!(self, "syn::ExprConst", syn::ExprConst, ExprConst, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_const_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_continue_mut(&mut self, i: &mut syn::ExprContinue) {
        do_work!(
            self,
            "syn::ExprContinue",
            syn::ExprContinue,
            ExprContinue,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_continue_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_field_mut(&mut self, i: &mut syn::ExprField) {
        do_work!(self, "syn::ExprField", syn::ExprField, ExprField, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_field_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_for_loop_mut(&mut self, i: &mut syn::ExprForLoop) {
        do_work!(self, "syn::ExprForLoop", syn::ExprForLoop, ExprForLoop, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_for_loop_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_group_mut(&mut self, i: &mut syn::ExprGroup) {
        do_work!(self, "syn::ExprGroup", syn::ExprGroup, ExprGroup, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_group_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_if_mut(&mut self, i: &mut syn::ExprIf) {
        do_work!(self, "syn::ExprIf", syn::ExprIf, ExprIf, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_if_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_index_mut(&mut self, i: &mut syn::ExprIndex) {
        do_work!(self, "syn::ExprIndex", syn::ExprIndex, ExprIndex, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_index_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_infer_mut(&mut self, i: &mut syn::ExprInfer) {
        do_work!(self, "syn::ExprInfer", syn::ExprInfer, ExprInfer, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_infer_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_let_mut(&mut self, i: &mut syn::ExprLet) {
        do_work!(self, "syn::ExprLet", syn::ExprLet, ExprLet, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_let_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_lit_mut(&mut self, i: &mut syn::ExprLit) {
        do_work!(self, "syn::ExprLit", syn::ExprLit, ExprLit, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_lit_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_loop_mut(&mut self, i: &mut syn::ExprLoop) {
        do_work!(self, "syn::ExprLoop", syn::ExprLoop, ExprLoop, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_loop_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_macro_mut(&mut self, i: &mut syn::ExprMacro) {
        do_work!(self, "syn::ExprMacro", syn::ExprMacro, ExprMacro, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_match_mut(&mut self, i: &mut syn::ExprMatch) {
        do_work!(self, "syn::ExprMatch", syn::ExprMatch, ExprMatch, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_match_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_method_call_mut(&mut self, i: &mut syn::ExprMethodCall) {
        do_work!(
            self,
            "syn::ExprMethodCall",
            syn::ExprMethodCall,
            ExprMethodCall,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_method_call_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_paren_mut(&mut self, i: &mut syn::ExprParen) {
        do_work!(self, "syn::ExprParen", syn::ExprParen, ExprParen, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_paren_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_path_mut(&mut self, i: &mut syn::ExprPath) {
        do_work!(self, "syn::ExprPath", syn::ExprPath, ExprPath, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_path_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_range_mut(&mut self, i: &mut syn::ExprRange) {
        do_work!(self, "syn::ExprRange", syn::ExprRange, ExprRange, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_range_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_raw_addr_mut(&mut self, i: &mut syn::ExprRawAddr) {
        do_work!(self, "syn::ExprRawAddr", syn::ExprRawAddr, ExprRawAddr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_raw_addr_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_reference_mut(&mut self, i: &mut syn::ExprReference) {
        do_work!(
            self,
            "syn::ExprReference",
            syn::ExprReference,
            ExprReference,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_reference_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_repeat_mut(&mut self, i: &mut syn::ExprRepeat) {
        do_work!(self, "syn::ExprRepeat", syn::ExprRepeat, ExprRepeat, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_repeat_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_return_mut(&mut self, i: &mut syn::ExprReturn) {
        do_work!(self, "syn::ExprReturn", syn::ExprReturn, ExprReturn, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_return_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_struct_mut(&mut self, i: &mut syn::ExprStruct) {
        do_work!(self, "syn::ExprStruct", syn::ExprStruct, ExprStruct, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_struct_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_try_mut(&mut self, i: &mut syn::ExprTry) {
        do_work!(self, "syn::ExprTry", syn::ExprTry, ExprTry, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_try_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_try_block_mut(&mut self, i: &mut syn::ExprTryBlock) {
        do_work!(
            self,
            "syn::ExprTryBlock",
            syn::ExprTryBlock,
            ExprTryBlock,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_try_block_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_tuple_mut(&mut self, i: &mut syn::ExprTuple) {
        do_work!(self, "syn::ExprTuple", syn::ExprTuple, ExprTuple, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_tuple_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_unary_mut(&mut self, i: &mut syn::ExprUnary) {
        do_work!(self, "syn::ExprUnary", syn::ExprUnary, ExprUnary, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_unary_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_unsafe_mut(&mut self, i: &mut syn::ExprUnsafe) {
        do_work!(self, "syn::ExprUnsafe", syn::ExprUnsafe, ExprUnsafe, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_unsafe_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_while_mut(&mut self, i: &mut syn::ExprWhile) {
        do_work!(self, "syn::ExprWhile", syn::ExprWhile, ExprWhile, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_while_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_expr_yield_mut(&mut self, i: &mut syn::ExprYield) {
        do_work!(self, "syn::ExprYield", syn::ExprYield, ExprYield, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_expr_yield_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_field_mut(&mut self, i: &mut syn::Field) {
        do_work!(self, "syn::Field", syn::Field, Field, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_field_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_field_mutability_mut(&mut self, i: &mut syn::FieldMutability) {
        do_work!(
            self,
            "syn::FieldMutability",
            syn::FieldMutability,
            FieldMutability,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_field_mutability_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_field_pat_mut(&mut self, i: &mut syn::FieldPat) {
        do_work!(self, "syn::FieldPat", syn::FieldPat, FieldPat, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_field_pat_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_field_value_mut(&mut self, i: &mut syn::FieldValue) {
        do_work!(self, "syn::FieldValue", syn::FieldValue, FieldValue, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_field_value_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_fields_mut(&mut self, i: &mut syn::Fields) {
        do_work!(self, "syn::Fields", syn::Fields, Fields, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_fields_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_fields_named_mut(&mut self, i: &mut syn::FieldsNamed) {
        do_work!(self, "syn::FieldsNamed", syn::FieldsNamed, FieldsNamed, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_fields_named_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_fields_unnamed_mut(&mut self, i: &mut syn::FieldsUnnamed) {
        do_work!(
            self,
            "syn::FieldsUnnamed",
            syn::FieldsUnnamed,
            FieldsUnnamed,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_fields_unnamed_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_file_mut(&mut self, i: &mut syn::File) {
        do_work!(self, "syn::File", syn::File, File, i);
        self.nested = 0;

        self.analyze_depth += 1;
        syn::visit_mut::visit_file_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_fn_arg_mut(&mut self, i: &mut syn::FnArg) {
        do_work!(self, "syn::FnArg", syn::FnArg, FnArg, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_fn_arg_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_foreign_item_mut(&mut self, i: &mut syn::ForeignItem) {
        do_work!(self, "syn::ForeignItem", syn::ForeignItem, ForeignItem, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_foreign_item_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_foreign_item_fn_mut(&mut self, i: &mut syn::ForeignItemFn) {
        do_work!(
            self,
            "syn::ForeignItemFn",
            syn::ForeignItemFn,
            ForeignItemFn,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_foreign_item_fn_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_foreign_item_macro_mut(&mut self, i: &mut syn::ForeignItemMacro) {
        do_work!(
            self,
            "syn::ForeignItemMacro",
            syn::ForeignItemMacro,
            ForeignItemMacro,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_foreign_item_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_foreign_item_static_mut(&mut self, i: &mut syn::ForeignItemStatic) {
        do_work!(
            self,
            "syn::ForeignItemStatic",
            syn::ForeignItemStatic,
            ForeignItemStatic,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_foreign_item_static_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_foreign_item_type_mut(&mut self, i: &mut syn::ForeignItemType) {
        do_work!(
            self,
            "syn::ForeignItemType",
            syn::ForeignItemType,
            ForeignItemType,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_foreign_item_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_generic_argument_mut(&mut self, i: &mut syn::GenericArgument) {
        do_work!(
            self,
            "syn::GenericArgument",
            syn::GenericArgument,
            GenericArgument,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_generic_argument_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_generic_param_mut(&mut self, i: &mut syn::GenericParam) {
        do_work!(
            self,
            "syn::GenericParam",
            syn::GenericParam,
            GenericParam,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_generic_param_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_generics_mut(&mut self, i: &mut syn::Generics) {
        do_work!(self, "syn::Generics", syn::Generics, Generics, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_generics_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_ident_mut(&mut self, i: &mut proc_macro2::Ident) {
        do_work!(self, "proc_macro2::Ident", proc_macro2::Ident, Ident, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_ident_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_impl_item_mut(&mut self, i: &mut syn::ImplItem) {
        do_work!(self, "syn::ImplItem", syn::ImplItem, ImplItem, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_impl_item_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_impl_item_const_mut(&mut self, i: &mut syn::ImplItemConst) {
        do_work!(
            self,
            "syn::ImplItemConst",
            syn::ImplItemConst,
            ImplItemConst,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_impl_item_const_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
        do_work!(self, "syn::ImplItemFn", syn::ImplItemFn, ImplItemFn, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_impl_item_fn_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_impl_item_macro_mut(&mut self, i: &mut syn::ImplItemMacro) {
        do_work!(
            self,
            "syn::ImplItemMacro",
            syn::ImplItemMacro,
            ImplItemMacro,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_impl_item_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_impl_item_type_mut(&mut self, i: &mut syn::ImplItemType) {
        do_work!(
            self,
            "syn::ImplItemType",
            syn::ImplItemType,
            ImplItemType,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_impl_item_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_impl_restriction_mut(&mut self, i: &mut syn::ImplRestriction) {
        do_work!(
            self,
            "syn::ImplRestriction",
            syn::ImplRestriction,
            ImplRestriction,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_impl_restriction_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_index_mut(&mut self, i: &mut syn::Index) {
        do_work!(self, "syn::Index", syn::Index, Index, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_index_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_mut(&mut self, i: &mut syn::Item) {
        do_work!(self, "syn::Item", syn::Item, Item, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_const_mut(&mut self, i: &mut syn::ItemConst) {
        do_work!(self, "syn::ItemConst", syn::ItemConst, ItemConst, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_const_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_enum_mut(&mut self, i: &mut syn::ItemEnum) {
        do_work!(self, "syn::ItemEnum", syn::ItemEnum, ItemEnum, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_enum_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_extern_crate_mut(&mut self, i: &mut syn::ItemExternCrate) {
        self.analyze_depth += 1;
        syn::visit_mut::visit_item_extern_crate_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_fn_mut(&mut self, i: &mut syn::ItemFn) {
        do_work!(self, "syn::ItemFn", syn::ItemFn, ItemFn, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_fn_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_foreign_mod_mut(&mut self, i: &mut syn::ItemForeignMod) {
        do_work!(
            self,
            "syn::ItemForeignMod",
            syn::ItemForeignMod,
            ItemForeignMod,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_foreign_mod_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_impl_mut(&mut self, i: &mut syn::ItemImpl) {
        do_work!(self, "syn::ItemImpl", syn::ItemImpl, ItemImpl, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_impl_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_macro_mut(&mut self, i: &mut syn::ItemMacro) {
        do_work!(self, "syn::ItemMacro", syn::ItemMacro, ItemMacro, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_mod_mut(&mut self, i: &mut syn::ItemMod) {
        do_work!(self, "syn::ItemMod", syn::ItemMod, ItemMod, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_mod_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_static_mut(&mut self, i: &mut syn::ItemStatic) {
        do_work!(self, "syn::ItemStatic", syn::ItemStatic, ItemStatic, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_static_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_struct_mut(&mut self, i: &mut syn::ItemStruct) {
        do_work!(self, "syn::ItemStruct", syn::ItemStruct, ItemStruct, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_struct_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_trait_mut(&mut self, i: &mut syn::ItemTrait) {
        do_work!(self, "syn::ItemTrait", syn::ItemTrait, ItemTrait, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_trait_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_trait_alias_mut(&mut self, i: &mut syn::ItemTraitAlias) {
        do_work!(
            self,
            "syn::ItemTraitAlias",
            syn::ItemTraitAlias,
            ItemTraitAlias,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_trait_alias_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_type_mut(&mut self, i: &mut syn::ItemType) {
        do_work!(self, "syn::ItemType", syn::ItemType, ItemType, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_union_mut(&mut self, i: &mut syn::ItemUnion) {
        do_work!(self, "syn::ItemUnion", syn::ItemUnion, ItemUnion, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_union_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_item_use_mut(&mut self, i: &mut syn::ItemUse) {
        do_work!(self, "syn::ItemUse", syn::ItemUse, ItemUse, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_item_use_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_label_mut(&mut self, i: &mut syn::Label) {
        do_work!(self, "syn::Label", syn::Label, Label, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_label_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lifetime_mut(&mut self, i: &mut syn::Lifetime) {
        do_work!(self, "syn::Lifetime", syn::Lifetime, Lifetime, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lifetime_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_lifetime_param_mut(&mut self, i: &mut syn::LifetimeParam) {
        do_work!(
            self,
            "syn::LifetimeParam",
            syn::LifetimeParam,
            LifetimeParam,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_lifetime_param_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_mut(&mut self, i: &mut syn::Lit) {
        do_work!(self, "syn::Lit", syn::Lit, Lit, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_bool_mut(&mut self, i: &mut syn::LitBool) {
        do_work!(self, "syn::LitBool", syn::LitBool, LitBool, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_bool_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_byte_mut(&mut self, i: &mut syn::LitByte) {
        do_work!(self, "syn::LitByte", syn::LitByte, LitByte, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_byte_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_byte_str_mut(&mut self, i: &mut syn::LitByteStr) {
        do_work!(self, "syn::LitByteStr", syn::LitByteStr, LitByteStr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_byte_str_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_cstr_mut(&mut self, i: &mut syn::LitCStr) {
        do_work!(self, "syn::LitCStr", syn::LitCStr, LitCStr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_cstr_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_char_mut(&mut self, i: &mut syn::LitChar) {
        do_work!(self, "syn::LitChar", syn::LitChar, LitChar, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_char_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_float_mut(&mut self, i: &mut syn::LitFloat) {
        do_work!(self, "syn::LitFloat", syn::LitFloat, LitFloat, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_float_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_int_mut(&mut self, i: &mut syn::LitInt) {
        do_work!(self, "syn::LitInt", syn::LitInt, LitInt, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_int_mut(self, i);
        self.analyze_depth -= 1;
    }
    fn visit_lit_str_mut(&mut self, i: &mut syn::LitStr) {
        do_work!(self, "syn::LitStr", syn::LitStr, LitStr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_lit_str_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_local_mut(&mut self, i: &mut syn::Local) {
        do_work!(self, "syn::Local", syn::Local, Local, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_local_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_local_init_mut(&mut self, i: &mut syn::LocalInit) {
        do_work!(self, "syn::LocalInit", syn::LocalInit, LocalInit, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_local_init_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_macro_mut(&mut self, i: &mut syn::Macro) {
        do_work!(self, "syn::Macro", syn::Macro, Macro, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_macro_delimiter_mut(&mut self, i: &mut syn::MacroDelimiter) {
        do_work!(
            self,
            "syn::MacroDelimiter",
            syn::MacroDelimiter,
            MacroDelimiter,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_macro_delimiter_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_member_mut(&mut self, i: &mut syn::Member) {
        do_work!(self, "syn::Member", syn::Member, Member, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_member_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_meta_mut(&mut self, i: &mut syn::Meta) {
        do_work!(self, "syn::Meta", syn::Meta, Meta, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_meta_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_meta_list_mut(&mut self, i: &mut syn::MetaList) {
        do_work!(self, "syn::MetaList", syn::MetaList, MetaList, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_meta_list_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_meta_name_value_mut(&mut self, i: &mut syn::MetaNameValue) {
        do_work!(
            self,
            "syn::MetaNameValue",
            syn::MetaNameValue,
            MetaNameValue,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_meta_name_value_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_parenthesized_generic_arguments_mut(
        &mut self,
        i: &mut syn::ParenthesizedGenericArguments,
    ) {
        self.analyze_depth += 1;
        syn::visit_mut::visit_parenthesized_generic_arguments_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_mut(&mut self, i: &mut syn::Pat) {
        do_work!(self, "syn::Pat", syn::Pat, Pat, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_ident_mut(&mut self, i: &mut syn::PatIdent) {
        do_work!(self, "syn::PatIdent", syn::PatIdent, PatIdent, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_ident_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_or_mut(&mut self, i: &mut syn::PatOr) {
        do_work!(self, "syn::PatOr", syn::PatOr, PatOr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_or_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_paren_mut(&mut self, i: &mut syn::PatParen) {
        do_work!(self, "syn::PatParen", syn::PatParen, PatParen, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_paren_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_reference_mut(&mut self, i: &mut syn::PatReference) {
        do_work!(
            self,
            "syn::PatReference",
            syn::PatReference,
            PatReference,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_reference_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_rest_mut(&mut self, i: &mut syn::PatRest) {
        do_work!(self, "syn::PatRest", syn::PatRest, PatRest, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_rest_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_slice_mut(&mut self, i: &mut syn::PatSlice) {
        do_work!(self, "syn::PatSlice", syn::PatSlice, PatSlice, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_slice_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_struct_mut(&mut self, i: &mut syn::PatStruct) {
        do_work!(self, "syn::PatStruct", syn::PatStruct, PatStruct, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_struct_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_tuple_mut(&mut self, i: &mut syn::PatTuple) {
        do_work!(self, "syn::PatTuple", syn::PatTuple, PatTuple, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_tuple_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_tuple_struct_mut(&mut self, i: &mut syn::PatTupleStruct) {
        do_work!(
            self,
            "syn::PatTupleStruct",
            syn::PatTupleStruct,
            PatTupleStruct,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_tuple_struct_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_type_mut(&mut self, i: &mut syn::PatType) {
        do_work!(self, "syn::PatType", syn::PatType, PatType, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pat_wild_mut(&mut self, i: &mut syn::PatWild) {
        do_work!(self, "syn::PatWild", syn::PatWild, PatWild, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_pat_wild_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_path_mut(&mut self, i: &mut syn::Path) {
        do_work!(self, "syn::Path", syn::Path, Path, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_path_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_path_arguments_mut(&mut self, i: &mut syn::PathArguments) {
        do_work!(
            self,
            "syn::PathArguments",
            syn::PathArguments,
            PathArguments,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_path_arguments_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_path_segment_mut(&mut self, i: &mut syn::PathSegment) {
        do_work!(self, "syn::PathSegment", syn::PathSegment, PathSegment, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_path_segment_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_pointer_mutability_mut(&mut self, i: &mut syn::PointerMutability) {
        do_work!(
            self,
            "syn::PointerMutability",
            syn::PointerMutability,
            PointerMutability,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_pointer_mutability_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_precise_capture_mut(&mut self, i: &mut syn::PreciseCapture) {
        do_work!(
            self,
            "syn::PreciseCapture",
            syn::PreciseCapture,
            PreciseCapture,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_precise_capture_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_predicate_lifetime_mut(&mut self, i: &mut syn::PredicateLifetime) {
        do_work!(
            self,
            "syn::PredicateLifetime",
            syn::PredicateLifetime,
            PredicateLifetime,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_predicate_lifetime_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_predicate_type_mut(&mut self, i: &mut syn::PredicateType) {
        do_work!(
            self,
            "syn::PredicateType",
            syn::PredicateType,
            PredicateType,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_predicate_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_qself_mut(&mut self, i: &mut syn::QSelf) {
        do_work!(self, "syn::QSelf", syn::QSelf, QSelf, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_qself_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_range_limits_mut(&mut self, i: &mut syn::RangeLimits) {
        do_work!(self, "syn::RangeLimits", syn::RangeLimits, RangeLimits, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_range_limits_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_receiver_mut(&mut self, i: &mut syn::Receiver) {
        do_work!(self, "syn::Receiver", syn::Receiver, Receiver, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_receiver_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_return_type_mut(&mut self, i: &mut syn::ReturnType) {
        do_work!(self, "syn::ReturnType", syn::ReturnType, ReturnType, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_return_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_signature_mut(&mut self, i: &mut syn::Signature) {
        do_work!(self, "syn::Signature", syn::Signature, Signature, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_signature_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_static_mutability_mut(&mut self, i: &mut syn::StaticMutability) {
        do_work!(
            self,
            "syn::StaticMutability",
            syn::StaticMutability,
            StaticMutability,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_static_mutability_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_stmt_mut(&mut self, i: &mut syn::Stmt) {
        do_work!(self, "syn::Stmt", syn::Stmt, Stmt, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_stmt_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_stmt_macro_mut(&mut self, i: &mut syn::StmtMacro) {
        do_work!(self, "syn::StmtMacro", syn::StmtMacro, StmtMacro, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_stmt_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_bound_mut(&mut self, i: &mut syn::TraitBound) {
        do_work!(self, "syn::TraitBound", syn::TraitBound, TraitBound, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_bound_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_bound_modifier_mut(&mut self, i: &mut syn::TraitBoundModifier) {
        do_work!(
            self,
            "syn::TraitBoundModifier",
            syn::TraitBoundModifier,
            TraitBoundModifier,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_bound_modifier_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_item_mut(&mut self, i: &mut syn::TraitItem) {
        do_work!(self, "syn::TraitItem", syn::TraitItem, TraitItem, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_item_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_item_const_mut(&mut self, i: &mut syn::TraitItemConst) {
        do_work!(
            self,
            "syn::TraitItemConst",
            syn::TraitItemConst,
            TraitItemConst,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_item_const_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_item_fn_mut(&mut self, i: &mut syn::TraitItemFn) {
        do_work!(self, "syn::TraitItemFn", syn::TraitItemFn, TraitItemFn, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_item_fn_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_item_macro_mut(&mut self, i: &mut syn::TraitItemMacro) {
        do_work!(
            self,
            "syn::TraitItemMacro",
            syn::TraitItemMacro,
            TraitItemMacro,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_item_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_trait_item_type_mut(&mut self, i: &mut syn::TraitItemType) {
        do_work!(
            self,
            "syn::TraitItemType",
            syn::TraitItemType,
            TraitItemType,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_trait_item_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_mut(&mut self, i: &mut syn::Type) {
        do_work!(self, "syn::Type", syn::Type, Type, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_array_mut(&mut self, i: &mut syn::TypeArray) {
        do_work!(self, "syn::TypeArray", syn::TypeArray, TypeArray, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_array_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_bare_fn_mut(&mut self, i: &mut syn::TypeBareFn) {
        do_work!(self, "syn::TypeBareFn", syn::TypeBareFn, TypeBareFn, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_bare_fn_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_group_mut(&mut self, i: &mut syn::TypeGroup) {
        do_work!(self, "syn::TypeGroup", syn::TypeGroup, TypeGroup, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_group_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_impl_trait_mut(&mut self, i: &mut syn::TypeImplTrait) {
        do_work!(
            self,
            "syn::TypeImplTrait",
            syn::TypeImplTrait,
            TypeImplTrait,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_impl_trait_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_infer_mut(&mut self, i: &mut syn::TypeInfer) {
        do_work!(self, "syn::TypeInfer", syn::TypeInfer, TypeInfer, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_infer_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_macro_mut(&mut self, i: &mut syn::TypeMacro) {
        do_work!(self, "syn::TypeMacro", syn::TypeMacro, TypeMacro, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_macro_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_never_mut(&mut self, i: &mut syn::TypeNever) {
        do_work!(self, "syn::TypeNever", syn::TypeNever, TypeNever, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_never_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_param_mut(&mut self, i: &mut syn::TypeParam) {
        do_work!(self, "syn::TypeParam", syn::TypeParam, TypeParam, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_param_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_param_bound_mut(&mut self, i: &mut syn::TypeParamBound) {
        do_work!(
            self,
            "syn::TypeParamBound",
            syn::TypeParamBound,
            TypeParamBound,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_param_bound_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_paren_mut(&mut self, i: &mut syn::TypeParen) {
        do_work!(self, "syn::TypeParen", syn::TypeParen, TypeParen, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_paren_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_path_mut(&mut self, i: &mut syn::TypePath) {
        do_work!(self, "syn::TypePath", syn::TypePath, TypePath, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_path_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_ptr_mut(&mut self, i: &mut syn::TypePtr) {
        do_work!(self, "syn::TypePtr", syn::TypePtr, TypePtr, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_ptr_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_reference_mut(&mut self, i: &mut syn::TypeReference) {
        do_work!(
            self,
            "syn::TypeReference",
            syn::TypeReference,
            TypeReference,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_reference_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_slice_mut(&mut self, i: &mut syn::TypeSlice) {
        do_work!(self, "syn::TypeSlice", syn::TypeSlice, TypeSlice, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_slice_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_trait_object_mut(&mut self, i: &mut syn::TypeTraitObject) {
        do_work!(
            self,
            "syn::TypeTraitObject",
            syn::TypeTraitObject,
            TypeTraitObject,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_trait_object_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_type_tuple_mut(&mut self, i: &mut syn::TypeTuple) {
        do_work!(self, "syn::TypeTuple", syn::TypeTuple, TypeTuple, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_type_tuple_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_un_op_mut(&mut self, i: &mut syn::UnOp) {
        do_work!(self, "syn::UnOp", syn::UnOp, UnOp, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_un_op_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_use_glob_mut(&mut self, i: &mut syn::UseGlob) {
        do_work!(self, "syn::UseGlob", syn::UseGlob, UseGlob, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_use_glob_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_use_group_mut(&mut self, i: &mut syn::UseGroup) {
        do_work!(self, "syn::UseGroup", syn::UseGroup, UseGroup, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_use_group_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_use_name_mut(&mut self, i: &mut syn::UseName) {
        do_work!(self, "syn::UseName", syn::UseName, UseName, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_use_name_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_use_path_mut(&mut self, i: &mut syn::UsePath) {
        do_work!(self, "syn::UsePath", syn::UsePath, UsePath, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_use_path_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_use_rename_mut(&mut self, i: &mut syn::UseRename) {
        do_work!(self, "syn::UseRename", syn::UseRename, UseRename, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_use_rename_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_use_tree_mut(&mut self, i: &mut syn::UseTree) {
        do_work!(self, "syn::UseTree", syn::UseTree, UseTree, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_use_tree_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_variadic_mut(&mut self, i: &mut syn::Variadic) {
        do_work!(self, "syn::Variadic", syn::Variadic, Variadic, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_variadic_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_variant_mut(&mut self, i: &mut syn::Variant) {
        do_work!(self, "syn::Variant", syn::Variant, Variant, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_variant_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_vis_restricted_mut(&mut self, i: &mut syn::VisRestricted) {
        do_work!(
            self,
            "syn::VisRestricted",
            syn::VisRestricted,
            VisRestricted,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_vis_restricted_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_visibility_mut(&mut self, i: &mut syn::Visibility) {
        do_work!(self, "syn::Visibility", syn::Visibility, Visibility, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_visibility_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_where_clause_mut(&mut self, i: &mut syn::WhereClause) {
        do_work!(self, "syn::WhereClause", syn::WhereClause, WhereClause, i);

        self.analyze_depth += 1;
        syn::visit_mut::visit_where_clause_mut(self, i);
        self.analyze_depth -= 1;
    }

    fn visit_where_predicate_mut(&mut self, i: &mut syn::WherePredicate) {
        do_work!(
            self,
            "syn::WherePredicate",
            syn::WherePredicate,
            WherePredicate,
            i
        );

        self.analyze_depth += 1;
        syn::visit_mut::visit_where_predicate_mut(self, i);
        self.analyze_depth -= 1;
    }
}
