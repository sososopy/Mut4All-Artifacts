//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Nullptr_In_Type_Check_159
 */ 
class MutatorFrontendAction_159 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(159)

private:
    class MutatorASTConsumer_159 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_159(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> candidatePointers;
        std::map<std::string, std::string> dummyDeclarations;
    };
};

//source file
#include "../include/Replace_Nullptr_In_Type_Check_159.h"

// ========================================================================================================
#define MUT159_OUTPUT 1

void MutatorFrontendAction_159::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if this is a type-checking function call like isa<>, dyn_cast<>, cast<>
      const clang::FunctionDecl *FD = CE->getDirectCallee();
      if (!FD) return;
      
      std::string funcName = FD->getNameInfo().getName().getAsString();
      if (funcName != "isa" && funcName != "dyn_cast" && funcName != "cast") return;
      
      // Check if first argument is nullptr
      if (CE->getNumArgs() < 1) return;
      clang::Expr *arg = CE->getArg(0);
      if (!arg) return;
      
      // Check if argument is nullptr
      if (auto *ILE = llvm::dyn_cast<clang::CXXNullPtrLiteralExpr>(arg->IgnoreImplicit())) {
        // Get the template argument type T from the type-checking function
        const clang::TemplateArgumentList *TAL = nullptr;
        if (auto *DRE = llvm::dyn_cast<clang::DeclRefExpr>(FD)) {
          if (auto *TSD = llvm::dyn_cast<clang::TemplateSpecializationType>(DRE->getType())) {
            if (TSD->template_arguments().size() > 0) {
              const clang::TemplateArgument &TA = TSD->template_arguments()[0];
              if (TA.getKind() == clang::TemplateArgument::Type) {
                clang::QualType templateType = TA.getAsType();
                
                // Collect candidate pointers from the translation unit
                candidatePointers.clear();
                clang::TranslationUnitDecl *TU = Result.Context->getTranslationUnitDecl();
                for (clang::Decl *D : TU->decls()) {
                  if (auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                    clang::QualType varType = VD->getType();
                    if (varType->isPointerType()) {
                      // Check if pointer type is incompatible with templateType
                      clang::QualType pointeeType = varType->getPointeeType();
                      if (!pointeeType.isNull() && !Result.Context->hasSameType(pointeeType, templateType)) {
                        candidatePointers.push_back(VD);
                      }
                    }
                  }
                }
                
                // Choose a replacement pointer
                std::string replacement;
                if (!candidatePointers.empty()) {
                  // Use existing pointer
                  size_t idx = getrandom::getRandomIndex(candidatePointers.size() - 1);
                  const clang::VarDecl *selectedVar = candidatePointers[idx];
                  replacement = selectedVar->getNameAsString();
                } else {
                  // Create dummy pointer declaration
                  std::string dummyName = "dummy_ptr_" + std::to_string(getrandom::getRandomIndex(1000));
                  if (dummyDeclarations.find(dummyName) == dummyDeclarations.end()) {
                    dummyDeclarations[dummyName] = "static int* " + dummyName + " = nullptr;";
                    // Insert dummy declaration at global scope
                    clang::TranslationUnitDecl *TU = Result.Context->getTranslationUnitDecl();
                    clang::SourceLocation insertLoc = TU->getEndLoc();
                    Rewrite.InsertTextBefore(insertLoc, "\n" + dummyDeclarations[dummyName] + "\n");
                  }
                  replacement = dummyName;
                }
                
                //Get the source code text of target node
                std::string sourceText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                                 clang::CharSourceRange::getTokenRange(arg->getSourceRange()));
                
                //Perform mutation on the source code text by applying string replacement
                std::string mutatedText = "/*mut159*/" + replacement;
                
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(arg->getSourceRange(), mutatedText);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_159::MutatorASTConsumer_159::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasArgument(0, cxxNullPtrLiteralExpr())).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}