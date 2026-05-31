//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Template_Recursion_Depth_Beyond_Compiler_Limits_156
 */ 
class MutatorFrontendAction_156 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(156)

private:
    class MutatorASTConsumer_156 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_156(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Expand_Template_Recursion_Depth_Beyond_Compiler_Limits_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getLocStart()))
        return;
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto &LO = Result.Context->getLangOpts();
      auto sourceRange = TE->getSourceRange();
      std::string originalText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(sourceRange), SM, LO).str();
      
      //Check if this is a function template specialization with non-type template arguments
      const clang::TemplateDecl* templateDecl = TE->getTemplateName().getAsTemplateDecl();
      if (!templateDecl) return;
      const clang::FunctionDecl* funcDecl = llvm::dyn_cast<clang::FunctionDecl>(templateDecl);
      if (!funcDecl) return;
      
      //Get template arguments
      const clang::TemplateArgumentList* args = TE->getTemplateArgs();
      if (!args) return;
      bool hasNonTypeIntArg = false;
      std::vector<std::pair<unsigned, llvm::APSInt>> intArgs;
      
      for (unsigned i = 0; i < args->size(); ++i) {
        if (args->get(i).getKind() == clang::TemplateArgument::Integral) {
          llvm::APSInt value = args->get(i).getAsIntegral();
          if (value.isSigned()) {
            hasNonTypeIntArg = true;
            intArgs.push_back(std::make_pair(i, value));
          }
        }
      }
      
      if (!hasNonTypeIntArg) return;
      
      //Choose a random integer argument to mutate
      if (intArgs.empty()) return;
      unsigned argIndex = getrandom::getRandomIndex(intArgs.size() - 1);
      unsigned templateArgPos = intArgs[argIndex].first;
      llvm::APSInt originalValue = intArgs[argIndex].second;
      
      //Generate new value: original + random offset (1-10)
      int offset = getrandom::getRandomIndex(9) + 1; // 1 to 10
      llvm::APSInt newValue = originalValue;
      llvm::APSInt offsetAPSInt(offset);
      newValue += offsetAPSInt;
      
      //Find the position of the template argument in the source text
      std::string templateArgsText = originalText.substr(originalText.find('<') + 1);
      templateArgsText = templateArgsText.substr(0, templateArgsText.rfind('>'));
      
      //Parse template arguments to find the right one
      std::vector<std::string> argStrings;
      std::string currentArg;
      int angleDepth = 0;
      for (char c : templateArgsText) {
        if (c == '<') angleDepth++;
        else if (c == '>') angleDepth--;
        else if (c == ',' && angleDepth == 0) {
          argStrings.push_back(currentArg);
          currentArg.clear();
          continue;
        }
        currentArg += c;
      }
      if (!currentArg.empty()) argStrings.push_back(currentArg);
      
      if (templateArgPos >= argStrings.size()) return;
      
      //Replace the argument value
      std::string oldArg = argStrings[templateArgPos];
      std::string newArg = std::to_string(newValue.getSExtValue());
      
      //Check if the argument is a constexpr variable reference
      size_t pos = originalText.find(oldArg, originalText.find('<'));
      if (pos == std::2::string::npos) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText;
      mutatedText.replace(pos, oldArg.length(), newArg);
      mutatedText = "/*mut156*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}