//header file
#pragma once
#include "Mutator_base.h"

/**
 * Corrupt_Template_Deduction_Guide_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingIdentifiers;
    };
};

//source file
#include "../include/Corrupt_Template_Deduction_Guide_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
      //Filter nodes in header files
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      if (!DG->isExplicit())
        return;
      //Get the source code text of target node
      auto sourceRange = DG->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Extract template class name from deduction guide
      auto deducedType = DG->getDeducedTemplate()->getDeclName();
      std::string className = deducedType.getAsString();
      //Generate random lowercase letter not in existing identifiers
      char randomLetter;
      do {
        randomLetter = 'a' + getrandom::getRandomIndex(25);
      } while (std::find(existingIdentifiers.begin(), existingIdentifiers.end(), std::string(1, randomLetter)) != existingIdentifiers.end());
      //Perform mutation on the source code text by applying string replacement
      size_t arrowPos = sourceText.find("->");
      if (arrowPos == std::string::npos)
        return;
      std::string newDeclarator = className + "() " + randomLetter;
      std::string mutatedText = sourceText.substr(0, arrowPos + 2) + " " + newDeclarator;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record existing identifiers
      if (VD->getIdentifier())
        existingIdentifiers.push_back(VD->getNameAsString());
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record existing identifiers
      if (FD->getIdentifier())
        existingIdentifiers.push_back(FD->getNameAsString());
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefNameDecl>("TypedefDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record existing identifiers
      if (TD->getIdentifier())
        existingIdentifiers.push_back(TD->getNameAsString());
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher deductionGuideMatcher = cxxDeductionGuideDecl().bind("DeductionGuide");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher typedefMatcher = typedefNameDecl().bind("TypedefDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(typedefMatcher, &callback);
    matchFinder.matchAST(Context);
}