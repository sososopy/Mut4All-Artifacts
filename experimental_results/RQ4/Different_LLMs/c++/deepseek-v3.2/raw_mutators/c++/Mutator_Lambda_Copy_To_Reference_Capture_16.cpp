//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Lambda_Copy_To_Reference_Capture_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Lambda_Copy_To_Reference_Capture_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Check if lambda has captures
      if (MT->getCaptureDefault() == clang::LCK_None && MT->capture_size() == 0)
        return;
      
      //Find copy captures to mutate
      bool hasCopyCapture = false;
      std::string mutatedText = lambdaText;
      size_t pos = 0;
      
      //Look for capture list
      size_t bracketStart = mutatedText.find('[');
      if (bracketStart == std::string::npos) return;
      size_t bracketEnd = mutatedText.find(']', bracketStart);
      if (bracketEnd == std::string::npos) return;
      
      std::string captureList = mutatedText.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
      std::string newCaptureList = "";
      size_t capturePos = 0;
      bool inDefaultCapture = false;
      
      //Parse capture list
      while (capturePos < captureList.length()) {
        //Skip whitespace
        while (capturePos < captureList.length() && std::isspace(captureList[capturePos])) {
          newCaptureList += captureList[capturePos];
          capturePos++;
        }
        
        if (capturePos >= captureList.length()) break;
        
        //Check for default capture
        if (captureList[capturePos] == '&' || captureList[capturePos] == '=') {
          newCaptureList += captureList[capturePos];
          capturePos++;
          inDefaultCapture = true;
          continue;
        }
        
        //Check for copy capture without '&'
        size_t captureStart = capturePos;
        while (capturePos < captureList.length() && captureList[capturePos] != ',' && captureList[capturePos] != ']' && !std::isspace(captureList[capturePos])) {
          capturePos++;
        }
        
        std::string capture = captureList.substr(captureStart, capturePos - captureStart);
        
        //Check if this is a copy capture (not reference and not 'this')
        if (capture != "this" && capture.find('&') == std::string::npos && !inDefaultCapture) {
          //Check if it's a capture with initializer
          size_t eqPos = capture.find('=');
          if (eqPos != std::string::npos) {
            //Capture with initializer: [var=expr]
            std::string varName = capture.substr(0, eqPos);
            std::string initExpr = capture.substr(eqPos);
            newCaptureList += "&" + varName + initExpr;
            hasCopyCapture = true;
          } else {
            //Simple capture: [var]
            newCaptureList += "&" + capture;
            hasCopyCapture = true;
          }
        } else {
          newCaptureList += capture;
        }
        
        //Add comma if present
        if (capturePos < captureList.length() && captureList[capturePos] == ',') {
          newCaptureList += ',';
          capturePos++;
          inDefaultCapture = false;
        }
      }
      
      if (hasCopyCapture) {
        //Perform mutation on the source code text by applying string replacement
        mutatedText.replace(bracketStart + system1, bracketEnd - bracketStart - 1, newCaptureList);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}