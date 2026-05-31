//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Template_Argument_Const_Qualification_153
 */ 
class MutatorFrontendAction_153 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(153)

private:
    class MutatorASTConsumer_153 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_153(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Template_Argument_Const_Qualification_153.h"

// ========================================================================================================
#define MUT153_OUTPUT 1

void MutatorFrontendAction_153::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto &LO = Result.Context->getLangOpts();
      std::string originalText = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(TS->getBeginLoc(), TS->getEndLoc()),
          SM, LO).str();
      
      //Perform mutation on the source code text by applying string replacement
      const clang::TemplateArgumentList &Args = TS->template_arguments();
      unsigned numArgs = Args.size();
      if (numArgs == 0) return;
      
      unsigned selectedArgIdx = getrandom::getRandomIndex(numArgs - 1);
      const clang::TemplateArgument &Arg = Args.get(selectedArgIdx);
      if (Arg.getKind() != clang::TemplateArgument::Type) return;
      
      clang::QualType ArgType = Arg.getAsType();
      if (ArgType->isReferenceType() || ArgType->isPointerType()) return;
      
      std::string mutatedText = originalText;
      std::string ArgTypeStr = ArgType.getAsString();
      std::string ArgTypeStrNoConst = ArgType.getNonReferenceType().getAsString();
      
      size_t templateStart = mutatedText.find('<');
      size_t templateEnd = mutatedText.rfind('>');
      if (templateStart == std::string::npos || templateEnd == std::string::npos) return;
      
      std::string innerArgs = mutatedText.substr(templateStart + 1, templateEnd - templateStart - 1);
      std::vector<std::string> argTokens;
      size_t pos = 0;
      size_t commaPos;
      while ((commaPos = innerArgs.find(',', pos)) != std::string::npos) {
          argTokens.push_back(innerArgs.substr(pos, commaPos - pos));
          pos = commaPos + 1;
      }
      argTokens.push_back(innerArgs.substr(pos));
      
      if (selectedArgIdx >= argTokens.size()) return;
      
      std::string selectedArgToken = argTokens[selectedArgIdx];
      std::string newArgToken;
      
      if (ArgType.isConstQualified()) {
          newArgToken = ArgTypeStrNoConst;
      } else {
          newArgToken = "const " + ArgTypeStr;
      }
      
      argTokens[selectedArgIdx] = newArgToken;
      std::string newInnerArgs;
      for (size_t i = 0; i < argTokens.size(); ++i) {
          newInnerArgs += argTokens[i];
          if (i != argTokens.size() - 1) newInnerArgs += ", ";
      }
      
      mutatedText.replace(templateStart + 1, templateEnd - templateStart - 1, newInnerArgs);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getBeginLoc(), TS->getEndLoc()), mutatedText);
    }
}
  
void MutatorFrontendAction_153::MutatorASTConsumer_153::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}