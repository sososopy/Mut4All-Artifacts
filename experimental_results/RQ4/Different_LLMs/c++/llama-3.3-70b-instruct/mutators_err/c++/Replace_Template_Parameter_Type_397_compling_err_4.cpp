//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Type_397
 */ 
class MutatorFrontendAction_397 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(397)

private:
    class MutatorASTConsumer_397 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_397(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Template_Parameter_Type_397.h"

// ========================================================================================================
#define MUT397_OUTPUT 1

void MutatorFrontendAction_397::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto &templateArgs = MT->getTemplateArgs();
      if (templateArgs.size() < 1) return;
      auto arg = templateArgs[0];
      //Perform mutation on the source code text by applying string replacement
      std::string newArg;
      if (arg->getAsType()->isIntegerType()) {
        newArg = "float";
      } else if (arg->getAsType()->isFloatingType()) {
        newArg = "int";
      } else {
        return;
      }
      std::string declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       MT->getSourceRange());
      size_t pos = declaration.find(arg->getAsType()->getAsString());
      if (pos!= std::string::npos) {
        declaration.replace(pos, arg->getAsType()->getAsString().length(), newArg);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_397::MutatorASTConsumer_397::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}