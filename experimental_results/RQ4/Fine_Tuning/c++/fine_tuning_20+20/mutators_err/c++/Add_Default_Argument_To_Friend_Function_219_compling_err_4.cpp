//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Default_Argument_To_Friend_Function_219
 */ 
class MutatorFrontendAction_219 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(219)

private:
    class MutatorASTConsumer_219 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_219(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/add_default_argument_to_friend_function_219.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Friend")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isInIdentifierNamespace(Decl::IDNS_OrdinaryFriend))
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->getDescribedFunctionTemplate() != nullptr)
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      if (params.empty())
        return;
      std::vector<int> non_default_param_index;
      for (std::size_t i = 0; i < params.size(); ++i) {
        if (!params[i]->hasDefaultArg())
          non_default_param_index.push_back(i);
      }
      if (non_default_param_index.empty())
        return;
      auto param_index = getrandom::getRandomIndex(non_default_param_index.size() - 1);
      auto target_index = non_default_param_index[param_index];
      auto target_param = params[target_index];
      auto target_type = target_param->getType();
      string default_value = "0";
      if (target_type->isBooleanType())
        default_value = "false";
      else if (target_type->isEnumeralType()) {
        auto ET = target_type->getAs<EnumType>();
        if (ET == nullptr)
          return;
        auto enum_decl = ET->getDecl();
        auto enum_name = enum_decl->getNameAsString();
        default_value = enum_name + "()";
      } else if (target_type->isPointerType()) {
        default_value = "nullptr";
      } else if (target_type->isFloatingType()) {
        default_value = "0.0";
      } else if (target_type->isIntegralType(*(Result.Context))) {
        default_value = "0";
      } else if (target_type->isRecordType()) {
        auto RT = target_type->getAs<RecordType>();
        if (RT == nullptr)
          return;
        auto record_decl = RT->getDecl();
        if (record_decl->isLambda())
          return;
        auto record_name = record_decl->getNameAsString();
        default_value = record_name + "()";
      } else if (target_type->isReferenceType()) {
        default_value = "*new int";
      } else if (target_type->isArrayType()) {
        default_value = "{}";
      } else if (target_type->isFunctionPointerType()) {
        default_value = "nullptr";
      } else if (target_type->isFunctionProtoType()) {
        default_value = "{}";
      } else if (target_type->isFunctionType()) {
        default_value = "{}";
      } else if (target_type->isUnionType()) {
        auto UT = target_type->getAs<clang::UnionType>();
        if (UT == nullptr)
          return;
        auto union_decl = UT->getDecl();
        auto union_name = union_decl->getNameAsString();
        default_value = union_name + "{}";
      } else if (target_type->isVectorType()) {
        default_value = "{}";
      } else if (target_type->isComplexType()) {
        default_value = "{}";
      } else if (target_type->isAtomicType()) {
        default_value = "{}";
      } else if (target_type->isBlockPointerType()) {
        default_value = "{}";
      } else if (target_type->isMemberPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectType()) {
        default_value = "{}";
      } else if (target_type->isObjCObjectPointerType()) {
        default_value = "{}";