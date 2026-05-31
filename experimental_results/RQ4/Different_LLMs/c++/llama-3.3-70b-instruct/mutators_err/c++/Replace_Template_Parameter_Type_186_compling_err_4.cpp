//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Type_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef file) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_186>(TheRewriter);
    }

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::TemplateParameterList *> templateParams;
    };
};

//source file
#include "../include/Replace_Template_Parameter_Type_186.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT186_OUTPUT 1

void MutatorFrontendAction_186::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TPL = Result.Nodes.getNodeAs<clang::TemplateParameterList>("TemplateParams")) {
      //Filter nodes in header files
      if (!TPL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TPL->getBeginLoc()))
        return;
      //Get the source code text of target node
      templateParams.push_back(TPL);
    } else if (auto *TTP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm")) {
      //Filter nodes in header files
      if (!TTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TTP->getBeginLoc()))
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string replacementType;
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        replacementType = "int";
      } else {
        replacementType = "double";
      }
      std::string originalType = TTP->getNameAsString();
      std::string mutatedCode = stringutils::rangetoStr(*(Result.SourceManager), TTP->getSourceRange());
      mutatedCode.replace(mutatedCode.find(originalType), originalType.length(), replacementType);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TTP->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_186::MutatorASTConsumer_186::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = cxxRecordDecl(has(templateParameterList())).bind("TemplateParams");
    DeclarationMatcher matcher2 = templateTypeParmDecl().bind("TemplateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}