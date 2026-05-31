//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_Underlying_Type_With_Incomplete_435
 */ 
class MutatorFrontendAction_435 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(435)

private:
    class MutatorASTConsumer_435 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_435(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> incompleteTypes;
    };
};

//source file
#include "../include/Mutator_Replace_Enum_Underlying_Type_With_Incomplete_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("IncompleteType")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        //Record incomplete class/struct declarations
        if (!RD->isCompleteDefinition()) {
            incompleteTypes.push_back(RD);
        }
    }
    else if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        //Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;
        //Check if enum has explicit underlying type
        if (!ED->getIntegerTypeSourceInfo())
            return;
        //Get the source code text of target node
        auto enumText = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
        //Find underlying type location
        auto typeSourceInfo = ED->getIntegerTypeSourceInfo();
        auto typeRange = typeSourceInfo->getTypeLoc().getSourceRange();
        auto underlyingTypeText = stringutils::rangetoStr(*(Result.SourceManager), typeRange);
        //Filter out incomplete types that are visible at enum location
        std::vector<const clang::CXXRecordDecl*> visibleIncompleteTypes;
        for (auto incompleteType : incompleteTypes) {
            //Check if incomplete type is visible at enum location
            if (Result.Context->getSourceManager().isBeforeInTranslationUnit(incompleteType->getLocation(), ED->getLocation())) {
                visibleIncompleteTypes.push_back(incompleteType);
            }
        }
        if (visibleIncompleteTypes.empty())
            return;
        //Select random incomplete type
        size_t idx = getrandom::getRandomIndex(visibleIncompleteTypes.size() - 1);
        auto selectedType = visibleIncompleteTypes[idx];
        //Perform mutation on the source code text by applying string replacement
        std::string newTypeName = selectedType->getNameAsString();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(typeRange, newTypeName);
    }
}
  
void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher enumMatcher = enumDecl(hasIntegerTypeSourceInfo()).bind("EnumDecl");
    DeclarationMatcher incompleteMatcher = cxxRecordDecl(unless(isDefinition())).bind("IncompleteType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(incompleteMatcher, &callback);
    matchFinder.matchAST(Context);
}