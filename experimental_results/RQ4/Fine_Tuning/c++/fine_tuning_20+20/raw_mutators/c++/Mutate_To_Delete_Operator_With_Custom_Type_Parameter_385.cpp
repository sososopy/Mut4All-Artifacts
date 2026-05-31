//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_To_Delete_Operator_With_Custom_Type_Parameter_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(385)

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<std::string> custom_classes;
    };
};

//source file
#include "../include/mutate_to_delete_operator_with_custom_type_parameter_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto DL_name = DL->getNameAsString();
      if (DL_name == "")
        return;
      if (DL->isAbstract())
        return;
      if (DL->hasUserDeclaredDestructor())
        return;
      custom_classes.push_back(DL_name);
      auto DL_dtor = DL->getDestructor();
      if (DL_dtor == nullptr)
        return;
      if (DL_dtor->hasBody())
        return;
      if (DL_dtor->isDeleted())
        return;
      if (DL_dtor->isDefaulted())
        return;
      if (DL_dtor->isUserProvided())
        return;
      if (DL_dtor->isUserDeclared())
        return;
      if (DL_dtor->isVirtual())
        return;
      if (DL_dtor->isTrivial())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (DL_dtor->isMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructor())
        return;
      if (DL_dtor->isCopyOrMoveAssignmentOperator())
        return;
      if (DL_dtor->isCopyOrMoveConstructorOrAssignmentOperator())
        return;
      if (DL_dtor->isOverloadedOperator())
        return;
      if (DL_dtor->isConversionFunction())
        return;
      if (DL_dtor->isStatic())
        return;
      if (DL_dtor->isConstexpr())
        return;
      if (DL_dtor->isConst())
        return;
      if (DL_dtor->isVolatile())
        return;
      if (DL_dtor->isVirtualAsWritten())
        return;
      if (DL_dtor->isDeletedAsWritten())
        return;
      if (DL_dtor->isDefaultedAsWritten())
        return;
      if (DL_dtor->isExplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isExplicitlyDefaulted())
        return;
      if (DL_dtor->isImplicit())
        return;
      if (DL_dtor->isPure())
        return;
      if (DL_dtor->isDefinedOutsideFunction())
        return;
      if (DL_dtor->isDefined())
        return;
      if (DL_dtor->isCopyConstructor())
        return;
      if (DL_dtor->isMoveConstructor())
        return;
      if (DL_dtor->isCopyAssignmentOperator())
        return;
      if (