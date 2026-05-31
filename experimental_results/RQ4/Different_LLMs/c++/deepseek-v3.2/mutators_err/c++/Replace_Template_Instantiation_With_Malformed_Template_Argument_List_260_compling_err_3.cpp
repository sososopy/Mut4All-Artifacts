//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Instantiation_With_Malformed_Template_Argument_List_260
 */ 
class MutatorFrontendAction_260 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(260)

private:
    class MutatorASTConsumer_260 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_260(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Template_Instantiation_With_Malformed_Template_Argument_List_260.h"

// ========================================================================================================
#define MUT260_OUTPUT 1

void MutatorFrontendAction_260::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getLocStart()))
        return;
      //Get the source code text of target node
      auto sourceRange = TE->getSourceRange();
      if (!sourceRange.isValid()) return;
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      size_t templateStart = sourceText.find('<');
      if (templateStart != std::string::npos) {
        sourceText.insert(templateStart + 1, "<");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_260::MutatorASTConsumer_260::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}