//source file
#include "../include/Mutator_Replace_Non_Type_Template_Argument_With_Scoped_Static_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record candidate variables that are at namespace scope or static members
      if ((VD->hasGlobalStorage() && VD->getType()->isIntegralOrEnumerationType()) || VD->getType()->isPointerType() || VD->getType()->isReferenceType()) {
        candidateVariables.push_back(VD);
      }
    }
    else if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateArgument>("TemplateArgument")) {
      //Filter nodes in header files
      if (!TE)
        return;
      
      //Get the source location from the template argument's expression if available
      clang::SourceLocation loc;
      if (TE->getKind() == clang::TemplateArgument::Integral) {
        if (auto *expr = TE->getAsExpr())
          loc = expr->getBeginLoc();
      } else if (TE->getKind() == clang::TemplateArgument::Declaration) {
        if (auto *decl = TE->getAsDecl())
          loc = decl->getLocation();
      }
      
      if (loc.isInvalid() || !Result.Context->getSourceManager().isWrittenInMainFile(loc))
        return;
      
      //Get the source code text of target node
      if (TE->getKind() != clang::TemplateArgument::Integral && TE->getKind() != clang::TemplateArgument::Declaration)
        return;
      
      //Find a candidate variable that matches the template argument type
      const clang::VarDecl* matchingVar = nullptr;
      for (const clang::VarDecl* VD : candidateVariables) {
        if (TE->getKind() == clang::TemplateArgument::Integral) {
          if (VD->getType()->isIntegralOrEnumerationType()) {
            matchingVar = VD;
            break;
          }
        } else if (TE->getKind() == clang::TemplateArgument::Declaration) {
          if (VD->getType()->isPointerType() || VD->getType()->isReferenceType()) {
            matchingVar = VD;
            break;
          }
        }
      }
      
      if (!matchingVar)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      //Create a new static variable inside a function
      std::string newStaticVar = "static " + matchingVar->getType().getAsString() + " scoped_static_" + matchingVar->getNameAsString() + " = ";
      if (TE->getKind() == clang::TemplateArgument::Integral) {
        newStaticVar += std::to_string(TE->getAsIntegral().getExtValue());
      } else {
        newStaticVar += matchingVar->getNameAsString();
      }
      newStaticVar += ";";
      
      //Wrap the template instantiation inside a new function if not already in a function
      clang::SourceManager& SM = Rewrite.getSourceMgr();
      clang::DeclContext* DC = Result.Context->getTranslationUnitDecl();
      bool isInFunction = false;
      
      //Check if we're inside a function by looking at the declaration context chain
      if (loc.isValid()) {
        auto decl = Result.Context->getTranslationUnitDecl();
        DC = decl;
        while (DC) {
            if (clang::isa<clang::FunctionDecl>(DC) || clang::isa<clang::CXXMethodDecl>(DC)) {
              isInFunction = true;
              break;
            }
            DC = DC->getParent();
        }
      }
      
      std::string mutation;
      if (!isInFunction) {
        mutation = "void mutator_function_448() {\n    " + newStaticVar + "\n    ";
        //Need to get the full template instantiation text
        clang::SourceRange range;
        if (auto *expr = TE->getAsExpr()) {
          range = expr->getSourceRange();
        } else if (auto *decl = TE->getAsDecl()) {
          range = decl->getSourceRange();
        }
        
        if (range.isValid()) {
          std::string templateText = stringutils::rangetoStr(SM, range);
          mutation += templateText + "\n}\n";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(range, mutation);
        }
      } else {
        mutation = newStaticVar + "\n    ";
        Rewrite.InsertTextBefore(loc, mutation);
      }
    }
}
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl(hasGlobalStorage()).bind("VarDecl");
    auto templateArgMatcher = templateArgumentLoc().bind("TemplateArgument");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(templateArgMatcher, &callback);
    matchFinder.matchAST(Context);
}