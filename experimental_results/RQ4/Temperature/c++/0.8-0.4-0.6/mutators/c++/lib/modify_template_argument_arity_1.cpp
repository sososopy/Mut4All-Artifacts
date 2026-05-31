//source file
#include "../include/modify_template_argument_arity_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *MC = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall")) {
      // Filter nodes in header files
      if (!MC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MC->getExprLoc()))
        return;

      if (auto *DRE = dyn_cast<DeclRefExpr>(MC->getCallee())) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->isTemplateInstantiation()) {
            // Get the template arguments from the callee's declaration
            if (auto *TemplateArgs = FD->getTemplateSpecializationArgs()) {
              std::string newTemplateArgs;
              if (TemplateArgs->size() > 0) {
                // Add an extra template argument to create an arity mismatch
                newTemplateArgs = "<int, ";
                for (unsigned i = 0; i < TemplateArgs->size(); ++i) {
                  if (i > 0) newTemplateArgs += ", ";
                  newTemplateArgs += TemplateArgs->get(i).getAsType().getAsString();
                }
                newTemplateArgs += ">";
              } else {
                // Add a single template argument if none exist
                newTemplateArgs = "<int>";
              }

              // Perform mutation on the source code text by applying string replacement
              SourceLocation LAngleLoc = MC->getBeginLoc().getLocWithOffset(1);
              SourceLocation RAngleLoc = MC->getEndLoc().getLocWithOffset(-1);
              SourceRange templateRange(LAngleLoc, RAngleLoc);
              Rewrite.ReplaceText(templateRange, newTemplateArgs);
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("TemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}