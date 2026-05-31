//header file
#pragma once
#include "Mutator_base.h"

/**
 * Corrupt_Template_Deduction_Guide_Syntax_280
 */ 
class MutatorFrontendAction_280 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(280)

private:
    class MutatorASTConsumer_280 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_280(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> templateClasses;
    };
};

//source file
#include "../include/Mutator_280.h"

// ========================================================================================================
#define MUT280_OUTPUT 1

void MutatorFrontendAction_280::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (!TD->isCompleteDefinition())
        return;
      if (!TD->getDescribedClassTemplate())
        return;
      //Record the node information to be used in the mutation process
      templateClasses.push_back(TD);
    }
    else if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
      //Filter nodes in header files
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      //Get the source code text of target node
      auto guideText = stringutils::rangetoStr(*(Result.SourceManager), DG->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> corruptions;
      corruptions.push_back(DG->getNameAsString() + "() " + DG->getNameAsString() + " c;");
      corruptions.push_back(DG->getNameAsString() + " " + DG->getNameAs2String() + " -> ;");
      corruptions.push_back(DG->getNameAsString() + "(T) ->");
      int choice = getrandom::getRandomIndex(corruptions.size() - 1);
      std::string corruptedGuide = corruptions[choice];
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DG->getSourceRange()), corruptedGuide);
    }
    else if (auto *InsertLoc = Result.Nodes.getNodeAs<clang::Decl>("InsertPoint")) {
      if (!InsertLoc || !Result.Context->getSourceManager().isWrittenInMainFile(
                     InsertLoc->getLocation()))
        return;
      if (templateClasses.empty())
        return;
      int idx = getrandom::getRandomIndex(templateClasses.size() - 1);
      const clang::CXXRecordDecl* targetClass = templateClasses[idx];
      std::string className = targetClass->getNameAsString();
      std::vector<std::string> corruptions;
      corruptions.push_back(className + "() " + className + " c;");
      corruptions.push_back(className + " " + className + " -> ;");
      corruptions.push_back(className + "(T) ->");
      int choice = getrandom::getRandomIndex(corruptions.size() - 1);
      std::string corruptedGuide = corruptions[choice];
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n/*mut280*/" + corruptedGuide;
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(InsertLoc->getEndLoc(), insertion);
    }
}
  
void MutatorFrontendAction_280::MutatorASTConsumer_280::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = cxxRecordDecl(hasAttr(clang::attr::Annotate)).bind("TemplateClass");
    DeclarationMatcher guideMatcher = cxxDeductionGuideDecl().bind("DeductionGuide");
    DeclarationMatcher insertMatcher = decl().bind("InsertPoint");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(guideMatcher, &callback);
    matchFinder.addMatcher(insertMatcher, &callback);
    matchFinder.matchAST(Context);
}