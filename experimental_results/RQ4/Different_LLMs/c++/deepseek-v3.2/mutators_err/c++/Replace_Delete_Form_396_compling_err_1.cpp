//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Delete_Form_396
 */ 
class MutatorFrontendAction_396 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(396)

private:
    class MutatorASTConsumer_396 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_396(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        bool hasGlobalDeleteDecl = false;
        bool mutationApplied = false;
    };
};

//source file
#include "../include/Mutator_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("GlobalDeleteDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        if (FD->isGlobal() && FD->getNameAsString().find("operator delete") != std::string::npos) {
            hasGlobalDeleteDecl = true;
        }
    }
    else if (auto *DE = Result.Nodes.getNodeAs<clang::CXXDeleteExpr>("DeleteExpr")) {
        //Filter nodes in header files
        if (!DE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DE->getLocation()))
            return;
        if (!hasGlobalDeleteDecl || mutationApplied)
            return;
        //Get the source code text of target node
        SourceRange range = DE->getSourceRange();
        std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText;
        if (DE->isArrayForm()) {
            // Replace delete[] with delete
            size_t pos = originalText.find("delete[]");
            if (pos != std::string::npos) {
                mutatedText = originalText;
                mutatedText.replace(pos, 8, "delete");
            }
        } else {
            // Replace delete with delete[]
            size_t pos = originalText.find("delete");
            if (pos != std::string::npos) {
                mutatedText = originalText;
                mutatedText.replace(pos, 6, "delete[]");
            }
        }
        //Replace the original AST node with the mutated one
        if (!mutatedText.empty()) {
            Rewrite.ReplaceText(range, mutatedText);
            mutationApplied = true;
        }
    }
}
  
void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher globalDeleteMatcher = functionDecl(isGlobal(), hasName("operator delete")).bind("GlobalDeleteDecl");
    StatementMatcher deleteExprMatcher = cxxDeleteExpr().bind("DeleteExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(globalDeleteMatcher, &callback);
    matchFinder.addMatcher(deleteExprMatcher, &callback);
    matchFinder.matchAST(Context);
}