//source file
#include "../include/Mutator_Template_Alias_In_Lambda_329.h"

// ========================================================================================================
#define MUT329_OUTPUT 1

void MutatorFrontendAction_329::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(TD);
    }
    else if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = LD->getSourceRange();
      if (lambdaRange.isInvalid())
        return;
      //Find enclosing template context
      const clang::DeclContext* ctx = LD->getLambdaClass()->getDeclContext();
      const clang::TemplateDecl* enclosingTemplate = nullptr;
      while (ctx) {
        if (const clang::TemplateDecl* td = llvm::dyn_cast<clang::TemplateDecl>(ctx)) {
          enclosingTemplate = td;
          break;
        }
        ctx = ctx->getParent();
      }
      if (!enclosingTemplate)
        return;
      //Find a template parameter from the enclosing template
      const clang::TemplateParameterList* params = enclosingTemplate->getTemplateParameters();
      if (!params || params->size() == 0)
        return;
      //Select a random template parameter
      size_t paramIdx = getrandom::getRandomIndex(params->size() - 1);
      const clang::NamedDecl* param = params->getParam(paramIdx);
      std::string paramName = param->getNameAsString();
      //Check if there's an existing using declaration inside the template that we can mutate
      bool foundUsing = false;
      std::string usingText;
      SourceLocation usingLoc;
      //Look for TypeAliasDecl within the template
      if (auto* td = llvm::dyn_cast<clang::TemplateDecl>(enclosingTemplate)) {
        if (auto* tdd = td->getTemplatedDecl()) {
          for (const clang::Decl* d : tdd->decls()) {
            if (auto* tad = llvm::dyn_cast<clang::TypeAliasDecl>(d)) {
              if (Result.Context->getSourceManager().isWrittenInMainFile(tad->getLocation())) {
                foundUsing = true;
                usingText = stringutils::rangetoStr(*(Result.SourceManager), tad->getSourceRange());
                usingLoc = tad->getBeginLoc();
                break;
              }
            }
          }
        }
      }
      //Perform mutation on the source code text by applying string replacement
      std::string mutation;
      if (foundUsing) {
        //Mutate existing using declaration
        //Replace the underlying type with _copy_cvref_t<paramName, originalType>
        //We need to extract the original type from the using declaration
        //Simplified approach: assume pattern "using Alias = Type;"
        size_t eqPos = usingText.find('=');
        if (eqPos != std::string::npos) {
          std::string beforeEq = usingText.substr(0, eqPos);
          std::string afterEq = usingText.substr(eqPos + 1);
          //Trim whitespace
          afterEq.erase(0, afterEq.find_first_not_of(" "));
          afterEq.erase(afterEq.find_last_not_of(" ") + 1);
          mutation = beforeEq + "= _copy_cvref_t<" + paramName + ", " + afterEq + ">;";
        } else {
          mutation = usingText;
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange(usingLoc, usingLoc.getLocWithOffset(usingText.length() - 1))), mutation);
      } else {
        //Insert new using declaration before the lambda
        //We need to find a suitable location within the template body
        SourceLocation insertLoc = LD->getBeginLoc();
        //Look for the start of the template body
        if (auto* fd = llvm::dyn_cast<clang::FunctionDecl>(enclosingTemplate->getTemplatedDecl())) {
          if (fd->hasBody()) {
            insertLoc = fd->getBody()->getBeginLoc().getLocWithOffset(1); //After opening brace
          }
        }
        //Create new using declaration
        mutation = "using MutAlias_329 = _copy_cvref_t<" + paramName + ", int>;\n";
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextBefore(insertLoc, "/*mut329*/" + mutation);
        //Also modify lambda to use the new alias if possible
        //This would require more complex AST manipulation; for simplicity we just insert the alias
      }
    }
}
  
void MutatorFrontendAction_329::MutatorASTConsumer_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    StatementMatcher lambdaMatcher = clang::ast_matchers::lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}