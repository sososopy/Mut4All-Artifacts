//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overload_Resolution_With_Mismatched_Template_Arguments_198
 */ 
class MutatorFrontendAction_198 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(198)

private:
    class MutatorASTConsumer_198 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_198(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const TypeDecl*> availableTypes;
    };
};

//source file
#include "../include/Mutator_Overload_Resolution_With_Mismatched_Template_Arguments_198.h"
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/Type.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>
#include <vector>
#include <set>
#include <random>

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if this is a template call with explicit template arguments
      if (auto *DRE = dyn_cast<DeclRefExpr>(CE->getCallee()->IgnoreImpCasts())) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate) {
            // Get the source location of template arguments
            SourceLocation TemplateArgLoc = DRE->getTemplateKeywordLoc();
            if (TemplateArgLoc.isValid()) {
              // Get the source range of the template arguments
              SourceRange TemplateArgRange = DRE->getSourceRange();
              // We need to find the actual template argument list
              if (auto *TemplateArgs = DRE->getTemplateArgs()) {
                // Collect all available types in scope
                availableTypes.clear();
                DeclContext *DC = FD->getDeclContext();
                while (DC) {
                  if (auto *TD = dyn_cast<TranslationUnitDecl>(DC)) {
                    for (auto *D : TD->decls()) {
                      if (auto *TypeD = dyn_cast<TypeDecl>(D)) {
                        availableTypes.push_back(TypeD);
                      }
                    }
                  } else if (auto *NS = dyn_cast<NamespaceDecl>(DC)) {
                    for (auto *D : NS->decls()) {
                      if (auto *TypeD = dyn_cast<TypeDecl>(D)) {
                        availableTypes.push_back(TypeD);
                      }
                    }
                  }
                  DC = DC->getParent();
                }
                
                // Get the original template arguments
                std::string OriginalArgs;
                for (unsigned i = 0; i < TemplateArgs->size(); ++i) {
                  const TemplateArgument &Arg = TemplateArgs->get(i);
                  if (Arg.getKind() == TemplateArgument::Type) {
                    QualType T = Arg.getAsType();
                    OriginalArgs += T.getAsString();
                    if (i != TemplateArgs->size() - 1) {
                      OriginalArgs += ", ";
                    }
                  }
                }
                
                // Select a replacement type
                if (!availableTypes.empty()) {
                  std::random_device rd;
                  std::mt19937 gen(rd());
                  std::uniform_int_distribution<size_t> dist(0, availableTypes.size() - 1);
                  size_t RandomIndex = dist(gen);
                  const TypeDecl *ReplacementType = availableTypes[RandomIndex];
                  std::string ReplacementName = ReplacementType->getNameAsString();
                  
                  // Create mutated template arguments
                  std::string MutatedArgs;
                  for (unsigned i = 0; i < TemplateArgs->size(); ++i) {
                    const TemplateArgument &Arg = TemplateArgs->get(i);
                    if (Arg.getKind() == TemplateArgument::Type) {
                      // Replace with the selected type
                      MutatedArgs += ReplacementName;
                      if (i != TemplateArgs->size() - 1) {
                        MutatedArgs += ", ";
                      }
                    }
                  }
                  
                  // Get the source range of the template argument list
                  SourceLocation StartLoc = DRE->getLAngleLoc();
                  SourceLocation EndLoc = DRE->getRAngleLoc();
                  if (StartLoc.isValid() && EndLoc.isValid()) {
                    //Get the source code text of target node
                    std::string OriginalText = std::string(Result.SourceManager->getCharacterData(StartLoc), Result.SourceManager->getCharacterData(EndLoc) - Result.SourceManager->getCharacterData(StartLoc) + 1);
                    
                    //Perform mutation on the source code text by applying string replacement
                    std::string MutatedText = "<" + MutatedArgs + ">";
                    
                    //Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), MutatedText);
                  }
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_198::MutatorASTConsumer_198::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = callExpr(hasAncestor(functionDecl().bind("funcDecl"))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}