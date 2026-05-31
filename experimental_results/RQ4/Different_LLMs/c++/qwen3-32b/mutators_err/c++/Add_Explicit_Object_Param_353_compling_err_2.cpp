//header file
#pragma once
#include "Mutator_base.h"
#include <vector>
#include <string>

/**
 * Add_Explicit_Object_Param_353
 */ 
class MutatorFrontendAction_353 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(353)

private:
    class MutatorASTConsumer_353 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_353(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingTypes;
    };
};

//source file
#include "../include/Add_Explicit_Object_Param_353.h"

// ========================================================================================================
#define MUT353_OUTPUT 1

void MutatorFrontendAction_353::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;
        if (recordDecl->isCompleteDefinition()) {
            existingTypes.push_back(recordDecl->getNameAsString());
        }
    } else if (auto *constructorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!constructorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(constructorDecl->getLocation()))
            return;

        const clang::FunctionDecl* funcDecl = constructorDecl;
        SourceLocation lParenLoc = funcDecl->getLParenLoc();
        SourceLocation rParenLoc = funcDecl->getRParenLoc();
        if (lParenLoc.isInvalid() || rParenLoc.isInvalid())
            return;

        std::string existingParamsText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getCharRange(lParenLoc, rParenLoc));

        if (existingTypes.empty())
            return;
        std::string selectedType = existingTypes[getrandom::getRandomIndex(existingTypes.size())];

        std::string newParams = "this " + selectedType + " &";
        if (!existingParamsText.empty()) {
            newParams += ", " + existingParamsText;
        }

        Rewrite.ReplaceText(CharSourceRange::getCharRange(lParenLoc, rParenLoc), newParams);
    }
}
  
void MutatorFrontendAction_353::MutatorASTConsumer_353::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("record");
    DeclarationMatcher constructorMatcher = cxxConstructorDecl().bind("constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(constructorMatcher, &callback);
    matchFinder.matchAST(Context);
}