//header file
#pragma once
#include "Mutator_base.h"

/**
 * vararg_operator_on_struct_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> structsWithOperators;
    };
};

//source file
#include "../include/vararg_operator_on_struct_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructWithOperator")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;

        // Check if the struct has a friend binary operator
        for (auto *Method : RD->methods()) {
            if (Method->isOverloadedOperator() && Method->hasAttr<clang::FriendAttr>()) {
                // Perform mutation on the source code text by applying string replacement
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager), Method->getSourceRange());
                if (declaration.find("operator,") != std::string::npos) {
                    std::string newDeclaration = "friend void operator,(" + RD->getNameAsString() + ", ...);";
                    Rewrite.ReplaceText(Method->getSourceRange(), newDeclaration);
                }
            }
        }
    }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxMethodDecl(isOverloadedOperator(), hasAttr<clang::FriendAttr>()))).bind("StructWithOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}