//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Target_Attribute_To_Cross_Architecture_401
 */ 
class MutatorFrontendAction_401 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(401)

private:
    class MutatorASTConsumer_401 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_401(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::string targetTriple;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Change_Target_Attribute_To_Cross_Architecture_401.h"

// ========================================================================================================
#define MUT401_OUTPUT 1

void MutatorFrontendAction_401::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("targetFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto attrs = FD->getAttrs();
      for (auto *attr : attrs) {
        if (auto *targetAttr = dyn_cast<clang::TargetAttr>(attr)) {
          std::string featuresStr = targetAttr->getFeaturesStr().str();
          if (featuresStr.length() > 0) {
            std::string currentFeature = featuresStr;
            std::string newFeature;
            //Determine cross-architecture feature based on target triple
            if (targetTriple.find("x86") != std::string::npos || targetTriple.find("amd64") != std::string::npos) {
              newFeature = "sve";
            } else if (targetTriple.find("aarch64") != std::string::npos || targetTriple.find("arm64") != std::string::npos) {
              newFeature = "avx512f";
            } else if (targetTriple.find("powerpc") != std::string::npos || targetTriple.find("ppc") != std::string::npos) {
              newFeature = "avx2";
            } else {
              newFeature = "unknown";
            }
            if (newFeature != "unknown" && newFeature != currentFeature) {
              //Perform mutation on the source code text by applying string replacement
              std::string attrText = stringutils::rangetoStr(*(Result.SourceManager), attr->getRange());
              std::size_t startPos = attrText.find(currentFeature);
              if (startPos != std::string::npos) {
                attrText.replace(startPos, currentFeature.length(), newFeature);
                attrText = "/*mut401*/" + attrText;
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(attr->getRange()), attrText);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_401::MutatorASTConsumer_401::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(clang::attr::Target)).bind("targetFunc");
    Callback callback(TheRewriter);
    callback.targetTriple = Context.getTargetInfo().getTriple().str();
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}