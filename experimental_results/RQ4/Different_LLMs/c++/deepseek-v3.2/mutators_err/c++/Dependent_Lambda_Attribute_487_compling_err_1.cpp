//header file
#pragma once
#include "Mutator_base.h"

/**
 * Dependent_Lambda_Attribute_487
 */ 
class MutatorFrontendAction_487 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(487)

private:
    class MutatorASTConsumer_487 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_487(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableStrings;
    };
};

//source file
#include "../include/Dependent_Lambda_Attribute_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("targetLambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      // Check if lambda is within a template context
      const DeclContext *DC = MT->getDecl()->getDeclContext();
      bool inTemplate = false;
      while (DC) {
        if (isa<FunctionDecl>(DC) && dyn_cast<FunctionDecl>(DC)->isTemplateDecl()) {
          inTemplate = true;
          break;
        }
        DC = DC->getParent();
      }
      if (!inTemplate) return;
      
      // Check for type-dependent parameters
      bool hasDependentParam = false;
      if (MT->getCallOperator()) {
        for (const ParmVarDecl *P : MT->getCallOperator()->parameters()) {
          if (P->getType()->isDependentType()) {
            hasDependentParam = true;
            break;
          }
        }
      }
      if (!hasDependentParam) return;
      
      // Check for annotate_type attribute
      bool hasAttribute = false;
      std::string attrText;
      SourceRange attrRange;
      if (MT->getDecl()) {
        for (const Attr *A : MT->getDecl()->getAttrs()) {
          if (A->getKind() == clang::attr::AnnotateType) {
            hasAttribute = true;
            attrText = stringutils::rangetoStr(*Result.SourceManager, A->getRange());
            attrRange = A->getRange();
            break;
          }
        }
      }
      if (!hasAttribute) return;
      
      // Collect available string literals from the translation unit
      if (availableStrings.empty()) {
        MatchFinder stringFinder;
        stringFinder.addMatcher(stringLiteral().bind("strLit"), [&](const MatchFinder::MatchResult &MR) {
          if (auto *SL = MR.Nodes.getNodeAs<clang::StringLiteral>("strLit")) {
            if (MR.Context->getSourceManager().isWrittenInMainFile(SL->getBeginLoc())) {
              availableStrings.push_back(SL->getString().str());
            }
          }
        });
        stringFinder.matchAST(*Result.Context);
        if (availableStrings.empty()) {
          availableStrings.push_back("mutated");
        }
      }
      
      //Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
      
      // Extract attribute argument string
      std::string origArg;
      size_t start = attrText.find('"');
      if (start != std::string::npos) {
        size_t end = attrText.find('"', start + 1);
        if (end != std::string::npos) {
          origArg = attrText.substr(start + 1, end - start - 1);
        }
      }
      
      // Choose mutation type
      int mutationType = getrandom::getRandomIndex(3);
      std::string mutatedText;
      
      if (mutationType == 0) { // Move attribute before capture
        size_t capturePos = lambdaText.find('[');
        if (capturePos != std::string::npos) {
          std::string attrWithoutArg = attrText;
          if (!origArg.empty()) {
            size_t argStart = attrWithoutArg.find('"');
            size_t argEnd = attrWithoutArg.find('"', argStart + 1);
            if (argEnd != std::string::npos) {
              attrWithoutArg.erase(argStart, argEnd - argStart + 1);
            }
          }
          mutatedText = lambdaText.substr(0, capturePos) + attrWithoutArg + " " + lambdaText.substr(capturePos);
        }
      } else if (mutationType == 1) { // Move attribute between parameter list and return type
        size_t paramEnd = lambdaText.find(')');
        if (paramEnd != std::string::npos) {
          std::string afterParams = lambdaText.substr(paramEnd + 1);
          size_t arrowPos = afterParams.find('->');
          if (arrowPos != std::string::npos) {
            mutatedText = lambdaText.substr(0, paramEnd + 1) + " " + attrText + " " + afterParams;
          } else {
            // Insert return type if not present
            mutatedText = lambdaText.substr(0, paramEnd + 1) + " " + attrText + " -> void " + afterParams;
          }
        }
      } else if (mutationType == 2) { // Move attribute after lambda body
        size_t bodyEnd = lambdaText.find('}');
        if (bodyEnd != std::string::npos) {
          mutatedText = lambdaText.substr(0, bodyEnd + 1) + " " + attrText + lambdaText.substr(bodyEnd + 1);
        }
      } else { // Change attribute argument
        std::string newArg;
        if (availableStrings.size() > 1) {
          // Choose a different string from available ones
          for (const std::string &s : availableStrings) {
            if (s != origArg) {
              newArg = s;
              break;
            }
          }
        } else {
          newArg = availableStrings[0];
        }
        
        size_t argStart = attrText.find('"');
        if (argStart != std::string::npos) {
          size_t argEnd = attrText.find('"', argStart + 1);
          if (argEnd != std::string::npos) {
            mutatedText = attrText.substr(0, argStart + 1) + newArg + attrText.substr(argEnd);
            // Replace the attribute in the lambda text
            size_t attrPos = lambdaText.find(attrText);
            if (attrPos != std::string::npos) {
              mutatedText = lambdaText.substr(0, attrPos) + mutatedText + lambdaText.substr(attrPos + attrText.length());
            }
          }
        }
      }
      
      //Replace the original AST node with the mutated one
      if (!mutatedText.empty()) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasDescendant(attr(annotateType()))).bind("targetLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}