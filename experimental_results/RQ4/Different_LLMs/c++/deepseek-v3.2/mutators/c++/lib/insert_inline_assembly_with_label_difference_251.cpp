//source file
#include "../include/Insert_Inline_Assembly_With_Label_Difference_251.h"

// ========================================================================================================
#define MUT251_OUTPUT 1

bool hasInlineAssembly(clang::Stmt *stmt) {
    if (!stmt) return false;
    if (auto *asmStmt = llvm::dyn_cast<clang::GCCAsmStmt>(stmt)) return true;
    if (auto *asmStmt = llvm::dyn_cast<clang::MSAsmStmt>(stmt)) return true;
    for (auto *child : stmt->children()) {
        if (hasInlineAssembly(child)) return true;
    }
    return false;
}

std::string generateUniqueLabel(std::set<std::string>& usedLabels) {
    std::string label;
    do {
        label = "__label_" + std::to_string(std::rand());
    } while (usedLabels.find(label) != usedLabels.end());
    usedLabels.insert(label);
    return label;
}

void MutatorFrontendAction_251::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->hasBody())
        return;
      //Check if function already contains inline assembly
      Stmt *Body = FD->getBody();
      if (Body && hasInlineAssembly(Body))
        return;
      //Generate unique labels
      std::string label1 = generateUniqueLabel(usedLabels);
      std::string label2 = generateUniqueLabel(usedLabels);
      //Construct inline assembly statement with label difference
      std::string asmStmt;
      //Assume ARM architecture for this example; could be extended for other targets
      asmStmt = "__asm__(\"" + label1 + ": movw r2, #:lower16:" + label2 + " - " + label1 + "\");";
      //Perform mutation on the source code text by applying string replacement
      SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
      std::string mutatedCode = "\n/*mut251*/" + asmStmt + "\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, mutatedCode);
    }
}
  
void MutatorFrontendAction_251::MutatorASTConsumer_251::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}