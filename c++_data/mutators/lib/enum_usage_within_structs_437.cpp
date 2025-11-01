//source file
#include "../include/Enum_Usage_Within_Structs_437.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructWithEnum")) {
        //Filter nodes in header files
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        for (const auto *D : SD->decls()) {
            if (const auto *ED = llvm::dyn_cast<clang::EnumDecl>(D)) {
                if (ED->enumerator_begin() != ED->enumerator_end()) {
                    EnumDeclNode = ED;
                    break;
                }
            }
        }
    }

    if (EnumDeclNode) {
        // Get the source code text of target node
        auto enumName = EnumDeclNode->getNameAsString();
        std::string usingEnumText = "using enum " + enumName + ";\n";
        
        // Perform mutation on the source code text by applying string replacement
        std::string compoundExpression = "int result = (";
        bool first = true;
        for (const auto *E : EnumDeclNode->enumerators()) {
            if (!first) {
                compoundExpression += " | ";
            }
            compoundExpression += E->getNameAsString();
            first = false;
        }
        compoundExpression += ");\n";

        // Replace the original AST node with the mutated one
        auto insertLoc = Result.Context->getTranslationUnitDecl()->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "/*mut437*/\n" + usingEnumText + compoundExpression);
    }
}
  
void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(enumDecl())).bind("StructWithEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}