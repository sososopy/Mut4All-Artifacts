//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Type_433
 */ 
class MutatorFrontendAction_Replace_Template_Parameter_Type_433 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Template_Parameter_Type_433)

private:
    class MutatorASTConsumer_Replace_Template_Parameter_Type_433 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Template_Parameter_Type_433(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_Type_433.h"

// ========================================================================================================
#define REPLACE_TEMPLATE_PARAMETER_TYPE_433_OUTPUT 1

void MutatorFrontendAction_Replace_Template_Parameter_Type_433::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string newType = "int"; // Replace with a different type
      size_t pos = declaration.find("auto");
      if (pos != std::string::npos) {
        declaration.replace(pos, 4, newType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_Replace_Template_Parameter_Type_433::MutatorASTConsumer_Replace_Template_Parameter_Type_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}