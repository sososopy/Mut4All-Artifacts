//source file
#include "../include/Modify_Array_Declaration_With_Negative_Size_349.h"

// ========================================================================================================
#define MUT349_OUTPUT 1

void MutatorFrontendAction_349::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      if (const clang::ConstantArrayType *CAT = Result.Context->getAsConstantArrayType(VD->getType())) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string negativeSize = "-5"; // Example negative size
        llvm::SmallVector<char, 16> originalSizeVec;
        CAT->getSize().toString(originalSizeVec, 10, false, false);
        std::string originalSize(originalSizeVec.begin(), originalSizeVec.end());
        size_t pos = declaration.find(originalSize);
        if (pos != std::string::npos) {
            declaration.replace(pos, originalSize.length(), negativeSize);
            // Insert a pragma to ensure the array is processed
            std::string pragma = "\n#pragma omp task depend(inout: " + VD->getNameAsString() + "[:][:])\n";
            declaration.insert(declaration.find(";") + 1, pragma);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_349::MutatorASTConsumer_349::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(constantArrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}