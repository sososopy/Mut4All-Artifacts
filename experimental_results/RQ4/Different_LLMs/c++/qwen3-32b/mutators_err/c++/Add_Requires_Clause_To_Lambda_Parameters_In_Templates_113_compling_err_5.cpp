//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Requires_Clause_To_Lambda_Parameters_In_Templates_113
 */ 
class MutatorFrontendAction_113 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(113)
private:
    class MutatorASTConsumer_113 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_113(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

//source file
#include "../include/Add_Requires_Clause_To_Lambda_Parameters_In_Templates_113.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT113_OUTPUT 1

void MutatorFrontendAction_113::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!record || !Result.Context->getSourceManager().isWrittenInMainFile(
                       record->getLocation()))
            return;
        availableTypes.push_back(record->getNameAsString());
    } else if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                       lambda->getCallOperator()->getLocation()))
            return;
        
        auto *callOp = lambda->getCallOperator();
        if (!callOp) return;
        
        bool hasAutoParam = false;
        std::string paramName;
        for (auto *param : callOp->parameters()) {
            if (param->getType()->getAs<clang::AutoType>()) {
                hasAutoParam = true;
                paramName = param->getNameAsString();
                break;
            }
        }
        if (!hasAutoParam) return;
        
        auto callOpRange = callOp->getSourceRange();
        auto callOpText = Rewrite.getRewrittenText(clang::CharSourceRange::getTokenRange(callOpRange));
        
        size_t openBracePos = callOpText.find("{");
        if (openBracePos == std::string::npos) return;
        
        if (availableTypes.empty()) return;
        std::string TName = availableTypes[std::rand() % availableTypes.size()];
        
        std::string requiresClause = " requires (requires { " + TName + "(" + paramName + "); })";
        std::string mutatedText = callOpText.substr(0, openBracePos) + requiresClause + callOpText.substr(openBracePos);
        
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(callOpRange), mutatedText);
    }
}

void MutatorFrontendAction_113::MutatorASTConsumer_113::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher typeMatcher = clang::ast_matchers::cxxRecordDecl().bind("record");
    clang::ast_matchers::DeclarationMatcher lambdaMatcher = clang::ast_matchers::lambdaExpr(
        clang::ast_matchers::anyOf(
            clang::ast_matchers::hasAncestor(clang::ast_matchers::functionTemplateDecl()),
            clang::ast_matchers::hasAncestor(clang::ast_matchers::classTemplateDecl())
        ),
        clang::ast_matchers::hasAnyParameter(clang::ast_matchers::hasType(clang::ast_matchers::autoType()))
    ).bind("lambda");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}