//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Compound_Assignment_Operand_Type_In_Templated_Wide_Integer_Expressions_478
 */ 
class MutatorFrontendAction_478 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(478)

private:
    class MutatorASTConsumer_478 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_478(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Compound_Assignment_Operand_Type_In_Templated_Wide_Integer_Expressions_478.h"

// ========================================================================================================
#define MUT478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("op")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getBeginLoc())) {
            return;
        }

        auto lhs = BO->getLHS()->IgnoreParenImpCasts();
        if (auto ME = dyn_cast<clang::MemberExpr>(lhs)) {
            auto base = ME->getBase()->IgnoreParenImpCasts();
            auto baseType = base->getType().getTypePtr();
            if (auto TST = dyn_cast<clang::TemplateSpecializationType>(baseType)) {
                auto templateName = TST->getTemplateName();
                if (auto TD = templateName.getAs<clang::TemplateDecl>()) {
                    if (TD->getName() != "WideInteger") {
                        return;
                    }
                } else {
                    return;
                }

                auto args = TST->template_arguments();
                if (args.size() < 2) {
                    return;
                }

                // Check if the second argument is an integer literal >= 128
                if (auto arg1 = args[1].getAsIntegral()) {
                    if (arg1.getValue().getSExtValue() < 128) {
                        return;
                    }
                } else {
                    return;
                }

                // Get the base type T
                auto T = args[0].getAsType();
                auto rhs = BO->getRHS()->IgnoreParenImpCasts();
                auto S = rhs->getType().getTypePtr();

                if (T == S) {
                    return;
                }

                // Replace RHS with reinterpret_cast<uint64_t>
                std::string replacement;
                if (auto DRE = dyn_cast<clang::DeclRefExpr>(rhs)) {
                    std::string varName = DRE->getNameInfo().getName().getAsString();
                    replacement = "reinterpret_cast<uint64_t>(" + varName + ")";
                } else {
                    return;
                }

                if (!replacement.empty()) {
                    Rewrite.ReplaceText(rhs->getSourceRange(), replacement);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = binaryOperator(
        hasAnyOperatorName("|=", "&=", "^=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="),
        hasLHS(memberExpr(hasType(templateSpecializationType(hasName("WideInteger"))))),
        hasRHS(declRefExpr(to(varDecl(hasType(isScalar())))))
    ).bind("op");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}