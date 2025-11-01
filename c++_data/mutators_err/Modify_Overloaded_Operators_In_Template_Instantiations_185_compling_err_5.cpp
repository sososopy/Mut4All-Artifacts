//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Modify_Overloaded_Operators_In_Template_Instantiations_185
 */ 
class MutatorFrontendAction_185 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(185)

private:
    class MutatorASTConsumer_185 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_185(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Overloaded_Operators_In_Template_Instantiations_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->isOverloadedOperator() && FD->getNumParams() == 2) {
            const auto *Param1Type = FD->getParamDecl(0)->getType().getTypePtr();
            const auto *Param2Type = FD->getParamDecl(1)->getType().getTypePtr();

            if (Param1Type->isBuiltinType() && Param2Type->isRecordType()) {
                if (auto *body = FD->getBody()) {
                    for (auto *stmt : body->children()) {
                        if (auto *returnStmt = dyn_cast<clang::ReturnStmt>(stmt)) {
                            std::string replacement = "U y; return y == x;";
                            Rewrite.ReplaceText(returnStmt->getSourceRange(), replacement);
                            break;
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isOverloadedOperator(), 
                                              hasParameter(0, hasType(builtinType())), 
                                              hasParameter(1, hasType(recordType())))
                                              .bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}