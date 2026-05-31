//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unexpected_Attribute_Specifier_434
 */ 
class MutatorFrontendAction_434 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(434)

private:
    class MutatorASTConsumer_434 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_434(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingAttributes;
        bool translationUnitEndProcessed = false;
    };
};

//source file
#include "../include/Mutator_Insert_Unexpected_Attribute_Specifier_434.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AD = Result.Nodes.getNodeAs<clang::Attr>("Attribute")) {
        //Filter nodes in header files
        if (!AD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AD->getLocation()))
            return;
        //Record existing attribute names
        std::string attrName = AD->getAttrName()->getName();
        existingAttributes.push_back(attrName);
    }
    else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
        //Filter nodes in header files
        if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
            return;
        //Ensure we only process the translation unit once
        if (translationUnitEndProcessed)
            return;
        translationUnitEndProcessed = true;
        
        //Get the source code text of target node
        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation endLoc = SM.getLocForEndOfFile(SM.getMainFileID());
        
        //Select attribute to insert
        std::string attributeToInsert;
        if (existingAttributes.empty()) {
            //Use common attributes if no existing ones found
            int choice = getrandom::getRandomIndex(2);
            if (choice == 0) attributeToInsert = "[[nodiscard]]";
            else if (choice == 1) attributeToInsert = "[[maybe_unused]]";
            else attributeToInsert = "[[deprecated]]";
        } else {
            //Use an existing attribute from the seed program
            int index = getrandom::getRandomIndex(existingAttributes.size() - 1);
            attributeToInsert = "[[" + existingAttributes[index] + "]]";
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string insertionText = "\n" + attributeToInsert;
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextBefore(endLoc, insertionText);
    }
}
  
void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher attrMatcher = attr().bind("Attribute");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(attrMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}