//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Arguments_164
 */ 
class MutatorFrontendAction_164 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(164)

private:
    class MutatorASTConsumer_164 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_164(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_164.h"

// ========================================================================================================
#define MUT164_OUTPUT 1

void MutatorFrontendAction_164::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT) return;
      //Get the source code text of target node
      auto TL = Result.Context->getTypeLocForUnqualified(MT);
      if (TL.isNull()) return;
      auto sourceRange = TL.getSourceRange();
      if (!sourceRange.isValid()) return;
      if (!Result.Context->getSourceManager().isWrittenInMainFile(
                     Result.SourceManager->getSpellingLoc(sourceRange.getBegin())))
        return;
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      // Find the position of the first '<'
      size_t startAngle = originalText.find('<');
      if (startAngle == std::string::npos) return;
      // Find the position of the last '>'
      size_t endAngle = originalText.rfind('>');
      if (endAngle == std::string::npos) return;
      // Determine mutation: remove all arguments or some arguments
      int choice = getrandom::getRandomIndex(1); // 0: remove all, 1: remove some
      std::string mutatedText;
      if (choice == 0) {
        mutatedText = originalText.substr(0, startAngle + 1) + ">" + originalText.substr(endAngle + 1);
      } else {
        // Remove some arguments: we need to parse the arguments
        std::string argsPart = originalText.substr(startAngle + 1, endAngle - startAngle - 1);
        // Split arguments by commas (simplified)
        std::vector<std::string> args;
        size_t pos = 0;
        while (pos < argsPart.length()) {
            size_t commaPos = argsPart.find(',', pos);
            if (commaPos == std::string::npos) {
                args.push_back(argsPart.substr(pos));
                break;
            }
            args.push_back(argsPart.substr(pos, commaPos - pos));
            pos = commaPos + 1;
        }
        if (args.size() <= 1) return; // Not enough arguments to remove some
        // Randomly select an argument to remove
        int removeIndex = getrandom::getRandomIndex(args.size() - 1);
        args[removeIndex] = "";
        // Reconstruct arguments part
        std::string newArgsPart;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i != 0) newArgsPart += ",";
            newArgsPart += args[i];
        }
        mutatedText = originalText.substr(0, startAngle + 1) + newArgsPart + ">" + originalText.substr(endAngle + 1);
      }
      mutatedText = "/*mut164*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_164::MutatorASTConsumer_164::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}