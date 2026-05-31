//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Make_Integer_Seq_Template_296
 */ 
class MutatorFrontendAction_296 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(296)

private:
    class MutatorASTConsumer_296 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_296(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl*> candidateTemplates;
    };
};

//source file
#include "../include/Mutator_Replace_Make_Integer_Seq_Template_296.h"

// ========================================================================================================
#define MUT296_OUTPUT 1

void MutatorFrontendAction_296::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record candidate templates
      candidateTemplates.push_back(TD);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Check if this is a __make_integer_seq call
      auto *Callee = CE->getDirectCallee();
      if (!Callee || !Callee->getIdentifier() || Callee->getName() != "__make_integer_seq")
        return;
      //Get the source code text of target node
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //Find template arguments
      auto Args = CE->getArgs();
      unsigned ArgCount = CE->getNumArgs();
      if (ArgCount < 3)
        return;
      //First argument should be a template template parameter
      auto *FirstArg = Args[0];
      if (!FirstArg->getType().getTypePtr()->isTemplateTypeParmType())
        return;
      //Get original template's parameter count
      auto *OriginalTemplate = FirstArg->getType().getTypePtr()->getAs<TemplateTypeParmType>()->getTemplateDecl();
      if (!OriginalTemplate)
         return;
      unsigned OriginalParamCount = OriginalTemplate->getTemplateParameters()->size();
      //Find a candidate template with different parameter count
      const clang::TemplateDecl* Replacement = nullptr;
      for (auto *Candidate : candidateTemplates) {
        unsigned CandidateParamCount = Candidate->getTemplateParameters()->size();
        if (CandidateParamCount != OriginalParamCount) {
          Replacement = Candidate;
          break;
        }
      }
      if (!Replacement)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string ReplacementName = Replacement->getNameAsString();
      std::string MutatedText = "__make_integer_seq<" + ReplacementName;
      //Keep other arguments unchanged
      for (unsigned i = 1; i < ArgCount; ++i) {
        std::string ArgText = stringutils::rangetoStr(*(Result.SourceManager), Args[i]->getSourceRange());
        MutatedText += ", " + ArgText;
      }
      MutatedText += ">";
      MutatedText = "/*mut296*/" + MutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), MutatedText);
    }
}
  
void MutatorFrontendAction_296::MutatorASTConsumer_296::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = clang::ast_matchers::decl(clang::ast_matchers::isTemplateDecl()).bind("TemplateDecl");
    StatementMatcher callExprMatcher = clang::ast_matchers::callExpr(clang::ast_matchers::callee(clang::ast_matchers::functionDecl(clang::ast_matchers::hasName("__make_integer_seq")))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}