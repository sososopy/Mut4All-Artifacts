//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_Count_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeDecl*> availableTypes;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Argument_Count_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record available type declarations for use as template arguments
      availableTypes.push_back(TD);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if this is a call to a function template with explicit template arguments
      if (auto *DRE = dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImpCasts())) {
        if (auto *FD = dyn_cast<clang::FunctionDecl>(DRE->getDecl())) {
          if (FD->getTemplatedKind() != clang::FunctionDecl::TK_NonTemplate) {
            // Get the source location of the template argument list
            SourceLocation TemplateLoc = DRE->getTemplateKeywordLoc();
            if (TemplateLoc.isValid()) {
              // Get the source range for the template argument list
              if (DRE->hasExplicitTemplateArgs()) {
                const auto *TemplateArgs = DRE->getTemplateArgs();
                unsigned NumArgs = TemplateArgs->size();
                SourceRange TemplateArgRange = DRE->getSourceRange();
                
                // Get the original template argument list text
                std::string TemplateArgsText = stringutils::rangetoStr(*(Result.SourceManager), TemplateArgRange);
                
                // Count the number of template arguments
                int argCount = 0;
                int angleDepth = 0;
                for (char c : TemplateArgsText) {
                  if (c == '<') angleDepth++;
                  else if (c == '>') angleDepth--;
                  else if (c == ',' && angleDepth == 1) argCount++;
                }
                if (argCount == 0 && Template1ArgsText.find('<') != std::string::npos) argCount = 1;
                
                // Decide whether to increase or decrease the argument count
                bool increase = getrandom::getRandomIndex(1) == 0;
                std::string newTemplateArgs;
                
                if (increase) {
                  // Increase argument count by adding another type
                  if (!availableTypes.empty()) {
                    size_t typeIndex = getrandom::getRandomIndex(availableTypes.size() - 1);
                    std::string typeName = availableTypes[typeIndex]->getNameAsString();
                    
                    // Insert new argument before the closing '>'
                    size_t lastComma = TemplateArgsText.rfind(',');
                    size_t closingAngle = TemplateArgsText.rfind('>');
                    
                    if (lastComma != std::string::npos) {
                      // Add after last comma
                      newTemplateArgs = TemplateArgsText;
                      newTemplateArgs.insert(lastComma + 1, " " + typeName + ",");
                    } else if (closingAngle != std::string::npos) {
                      // Add before closing angle bracket
                      newTemplateArgs = TemplateArgsText;
                      newTemplateArgs.insert(closingAngle, ", " + typeName);
                    }
                  }
                } else {
                  // Decrease argument count by removing the last argument
                  if (argCount > 1) {
                    size_t lastComma = TemplateArgsText.rfind(',');
                    size_t closingAngle = TemplateArgsText.rfind('>');
                    
                    if (lastComma != std::string::npos && closingAngle != std::string::npos) {
                      // Find the start of the last argument
                      size_t argStart = lastComma;
                      while (argStart > 0 && TemplateArgsText[argStart-1] == ' ') argStart--;
                      
                      newTemplateArgs = TemplateArgsText.substr(0, argStart) + 
                                        TemplateArgsText.substr(closingAngle);
                    }
                  }
                }
                
                // Perform mutation if we have a1 valid new template argument list
                if (!newTemplateArgs.empty()) {
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(TemplateArgRange, "/*mut196*/" + newTemplateArgs);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher typeMatcher = clang::ast_matchers::typeDecl().bind("TypeDecl");
    StatementMatcher callMatcher = clang::ast_matchers::callExpr(clang::ast_matchers::hasAncestor(clang::ast_matchers::functionDecl())).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}