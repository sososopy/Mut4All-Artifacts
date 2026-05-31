//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Non_Type_Parameter_With_Pointer_Dereference_In_Deduced_Context_374
 */ 
class MutatorFrontendAction_374 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(374)

private:
    class MutatorASTConsumer_374 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_374(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Non_Type_Parameter_With_Pointer_Dereference_In_Deduced_Context_374.h"

// ========================================================================================================
#define MUT374_OUTPUT 1

void MutatorFrontendAction_374::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateArgument>("templateArg")) {
      //Filter nodes in header files
      if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getLocation()))
        return;
      //Get the source code text of target node
      auto *TP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("templateParam");
      if (!TP) return;
      if (TE->getKind() != clang::TemplateArgument::Integral) return;
      auto paramName = TP->getNameAsString();
      if (paramName.empty()) return;
      auto sourceRange = TE->getSourceRange();
      if (!sourceRange.isValid()) return;
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      string mutatedText = "*" + paramName;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_374::MutatorASTConsumer_374::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateArgument(hasParent(templateSpecializationType()), refersTo(nonTypeTemplateParmDecl(hasType(autoType())))).bind("templateArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}