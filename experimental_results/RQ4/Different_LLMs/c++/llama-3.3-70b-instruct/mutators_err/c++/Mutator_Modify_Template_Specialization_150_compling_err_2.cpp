//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_150
 */ 
class MutatorFrontendAction_150 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_150>(TheRewriter);
    }

private:
    class MutatorASTConsumer_150 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_150(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl *> templateSpecializations;
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Modify_Template_Specialization_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TS ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      //Get the source code text of target node
      templateSpecializations.push_back(TS);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDeclaration")) {
      //Filter nodes in header files
      if (!TD ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (!templateSpecializations.empty()) {
        // Perform mutation on the source code text by applying string replacement
        auto newType = getNewType(templateSpecializations[0]);
        if (newType!= "") {
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   templateSpecializations[0]->getSourceRange());
          auto newDeclaration = replaceType(declaration, newType);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(templateSpecializations[0]->getSourceRange()), newDeclaration);
        }
      }
    }
}

void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    DeclarationMatcher matcher2 = classTemplateDecl().bind("TemplateDeclaration");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}

std::string getNewType(const clang::ClassTemplateSpecializationDecl *TS) {
  // Implement logic to get a new type that already exists in the seed program
  // or introduce a new type similar to the original type
  // For simplicity, this example returns an empty string
  return "";
}

std::string replaceType(const std::string &declaration, const std::string &newType) {
  // Implement logic to replace the original type with the new type in the declaration
  // For simplicity, this example returns the original declaration
  return declaration;
}