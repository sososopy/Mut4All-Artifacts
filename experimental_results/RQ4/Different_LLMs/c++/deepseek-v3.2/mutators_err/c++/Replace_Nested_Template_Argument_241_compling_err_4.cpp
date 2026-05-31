//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Nested_Template_Argument_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(241)

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Nested_Template_Argument_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecializationType")) {
      //Filter nodes in header files
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TT->getTypeLoc().getBeginLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = TT->getTypeLoc().getSourceRange();
      if (!sourceRange.isValid()) return;
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      // Find the last '>' in the template argument list
      size_t lastAnglePos = originalText.find_last_of('>');
      if (lastAnglePos == std::string::npos) return;
      // Insert an extra '>' after the existing one
      std::string mutatedText = originalText;
      mutatedText.insert(lastAnglePos + 1, ">");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecializationType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}